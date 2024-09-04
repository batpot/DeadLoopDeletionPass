#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopUnrollAnalyzer.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/LoopPeel.h"
#include "llvm/Transforms/Utils/LoopSimplify.h"
#include "llvm/Transforms/Utils/SizeOpts.h"
#include "llvm/Transforms/Utils/UnrollLoop.h"

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/Support/raw_ostream.h"


#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IRBuilder.h"

#include <vector>
#include <map>
#include <iostream>

using namespace llvm;


namespace {
    struct DeadLoopDeletionPass : public LoopPass {
        std::vector<BasicBlock *> LoopBasicBlocks;
        std::vector<BasicBlock *> LoopBodyBasicBlocks;

        std::unordered_map<Value *, Value *> VariablesMap;
        std::unordered_map<Value *, Value *> FinalValues;

        static char ID;
        DeadLoopDeletionPass() : LoopPass(ID) {}


        void mapVariables() {
            for (BasicBlock *BB : LoopBodyBasicBlocks) {
                for (Instruction &I : *BB) {
                    if (isa<LoadInst>(&I)) {
                        VariablesMap[&I] = I.getOperand(0);
                    }
                }
            }
        }

        // hocemo da brisemo petlje koje 
        // - nikad se ne izvrsavaju 
        // - se izvrsavaju ali ne uticu na rezultat 
        // - nemaju bocne efekte


        void printVariablesMap() {
            // Prolazi kroz mapu i ispisuje sve parove ključ-vrednost
            for (const auto &entry : VariablesMap) {
                Value *fst = entry.first;
                Value *value = entry.second;
                
                // Ispisuje adresu instrukcije i vrednost (operand)
                errs() << "Value: " << *fst << "\n";
                errs() << "Mapped to: " << *value << "\n";
            }
        }

        void printFinalValuesMap() {
            // Prolazi kroz mapu i ispisuje sve parove ključ-vrednost
            for (const auto &entry : FinalValues) {
                Value *fst = entry.first;
                Value *value = entry.second;
                
                // Ispisuje adresu instrukcije i vrednost (operand)
                errs() << "Value: " << *fst << "\n";
                errs() << "Mapped to: " << *value << "\n";
            }
        }


        bool isConstInt(Value *Operand)
        {
            return isa<ConstantInt>(Operand);
        }

        int getValueFromConstInt(Value *Operand)
        {
            ConstantInt *ConstInt = dyn_cast<ConstantInt>(Operand);
            return ConstInt->getSExtValue();
        }


        void saveFinalValues() {
            for (BasicBlock *BB : LoopBodyBasicBlocks) {
                for (Instruction &I : *BB) {
                    if (StoreInst *storeInst = dyn_cast<StoreInst>(&I)) {
                        Value *ptr = storeInst->getOperand(1);      
                        Value *value = storeInst->getOperand(0); // Vrednost koja se upisuje
                        // errs() << "FINAL VAAAAL : " << *ptr << "\n";
                        // errs() << "FV MAPPED TO: " << *value << "\n";
                        FinalValues[ptr] = value;
                    }
                }
            }
        }


        bool checkIfHasCall() {
            for (BasicBlock *BB : LoopBodyBasicBlocks) {
                for (Instruction &I : *BB) {
                    if (auto *Call = dyn_cast<CallInst>(&I)) {
                        return true;
                    }
                }
            }
            return false;
        }

        Value* ResolveToBaseVariable(Value *val) {
            if (LoadInst *loadInst = dyn_cast<LoadInst>(val)) {
                Value *ptr = loadInst->getPointerOperand();
                return ResolveToBaseVariable(ptr); // Rekurzivno prati do osnovne promenljive
            }
            return val; // Ako nije load instrukcija, vrati trenutnu vrednost
        }

        bool isUseful(Value *I) { 
            if (auto *op = dyn_cast<BinaryOperator>(I)) {
                if (op->getOpcode() == Instruction::Add || op->getOpcode() == Instruction::Sub) {
                    
                    // Proveri da li je neki od operanada 0
                    for (unsigned i = 0; i < op->getNumOperands(); ++i) {
                        if (ConstantInt *constInt = dyn_cast<ConstantInt>(op->getOperand(i))) {
                            if (constInt->isZero()) {
                                return false; // Nekorisna instrukcija
                            }
                        }
                    }
                }
            
                if (op->getOpcode() == Instruction::Mul) {
                    if (ConstantInt *C = dyn_cast<ConstantInt>(op->getOperand(1))) {
                        if (C->isOne()) {
                            return false; // Nekorisno ako je množenje sa 1
                        }
                    }
                }

                if (op->getOpcode() == Instruction::SDiv || op->getOpcode() == Instruction::UDiv) {
                    if (ConstantInt *C = dyn_cast<ConstantInt>(op->getOperand(1))) {
                        if (C->isOne()) {
                            return false; // Nekorisno ako je deljenje sa 1
                        }
                    }
                }
            }

            return true;
        }


        bool shouldDelete() {
            for (const auto &entry : VariablesMap) {
                Value *InitialPtr = entry.first;
                Value *InitialValue = entry.second;
                // errs() << "Valueeeeeeeeee: " << *InitialPtr << "\n";
                // errs() << "Mapped toooooo: " << *InitialValue << "\n";

                Value *ResolvedPtr = ResolveToBaseVariable(InitialValue);

                // errs() << "Resolved toooo: " << *ResolvedPtr << "\n";

                if (FinalValues.find(ResolvedPtr) != FinalValues.end()) {
                    if(isUseful(FinalValues[ResolvedPtr]))
                        return false;           // ako je bas jedna instrukcija u bloku korisna, ne smemo brisati petlju
                }
            }
            
            return true;  // Vrednosti su ostale iste
        }


        bool DeleteLoopIfDead(Loop *L) {
            if(checkIfHasCall())
                return false;   // petlja ima poziv printf pa nije dead loop

            if(!shouldDelete())
                return false;   // ako su se vrednosti menjale checkIfValuesChanged vraca true pa ne treba da brisemo petlju

            DeleteDeadLoop(L);
            return true;
        }

        
        // brise petlju skroz 
        void DeleteDeadLoop(Loop *L) {
            std::vector<BasicBlock *> LoopBodyBasicBlocks(LoopBasicBlocks.size() - 2);
            std::copy(LoopBasicBlocks.begin() + 1, LoopBasicBlocks.end() - 1, LoopBodyBasicBlocks.begin()); // [)

            L->getLoopPreheader()->getTerminator()->setSuccessor(0, L->getExitBlock());
            
            for(BasicBlock *BB : LoopBasicBlocks) {
                BB->eraseFromParent();
            }
        }


        bool runOnLoop(Loop *L, LPPassManager &LPM) override {
            VariablesMap.clear();
            FinalValues.clear();

            LoopBodyBasicBlocks.clear();

            LoopBasicBlocks = L->getBlocksVector();

            BasicBlock *Header = L->getHeader();
            BasicBlock *Latch = L->getLoopLatch();

            // sredisnji blokovi bez header i latch
            for (BasicBlock *BB : LoopBasicBlocks) {
                if (BB != Latch) {
                    LoopBodyBasicBlocks.push_back(BB);
                }
            }

            //printf("%d aaaaa\n", LoopBodyBasicBlocks.size());

            mapVariables();

            saveFinalValues();

            bool res = DeleteLoopIfDead(L);
            if(res)
                std::cout << "petlja JE obrisana\n";
            else
                std::cout << "petlja NIJE obrisana\n";

            //Result == obrisana petlja promenjen IR
            return res;
        }

    };

}

char DeadLoopDeletionPass::ID = 0;
static RegisterPass<DeadLoopDeletionPass> X("dead-loops", "This pass removes loops that are useles or without side effects!");

