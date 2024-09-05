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

    	int BoundValue;

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


        void saveFinalValues() {
            for (BasicBlock *BB : LoopBodyBasicBlocks) {
                for (Instruction &I : *BB) {
                    if (StoreInst *storeInst = dyn_cast<StoreInst>(&I)) {
                        Value *ptr = storeInst->getOperand(1);      
                        Value *value = storeInst->getOperand(0); // Vrednost koja se upisuje
                        //errs() << "FINAL VAAAAL : " << *ptr << "\n";
                        //errs() << "FV MAPPED TO: " << *value << "\n\n";
                        FinalValues[ptr] = value;
                    }
                }
            }
        }

        // hocemo da brisemo petlje koje 
        // - nikad se ne izvrsavaju 
        // - se izvrsavaju ali ne uticu na rezultat 
        // - nemaju bocne efekte

        // PROVERA ZA LOOP KOJI SE NIKAD NE IZVRSAVA

        void findLoopCounterAndBound(Loop *L)
        {
        	for (Instruction &I : *L->getHeader()) {
        		if (isa<ICmpInst>(&I)) {
        			if (ConstantInt *ConstInt = dyn_cast<ConstantInt>(I.getOperand(1)))
        				BoundValue = ConstInt->getSExtValue();
        		}
        	}
        }

        bool isNeverExecuted(Loop *L)
        {
            // TODO uopsti da ceo uslov bude neki false condition kao sto je i<0
			return (BoundValue == 0) ? true : false;
			
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


        bool checkIfHasCall() {
            for (BasicBlock *BB : LoopBodyBasicBlocks) {
                for (Instruction &I : *BB) {
                    if(isa<CallInst>(&I)) {
                        return true;
                    }
                }
            }
            return false;
        }


        Value* Resolve(Value *val) {
            if (LoadInst *loadInst = dyn_cast<LoadInst>(val)) {
                Value *ptr = loadInst->getPointerOperand();
                return Resolve(ptr); // Rekurzivno prati do osnovne promenljive
            }
            if(FinalValues[val])
                return FinalValues[val];

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
                    for (int i = 0; i < 2; i++) {
                        if (ConstantInt *C = dyn_cast<ConstantInt>(op->getOperand(i))) {
                            if (C->isOne()) {
                                return false; // Nekorisno ako je množenje sa 1
                            }
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


        bool hasArithmeticOperations()
        {
          // ako ima samo store brisemo
            for (BasicBlock *BB : LoopBodyBasicBlocks) {
                for (Instruction &I : *BB) {
                    if (isa<BinaryOperator>(&I)) {
                        if (isa<AddOperator>(&I) || isa<MulOperator>(&I)
                              || isa<SDivOperator>(&I) || isa<SubOperator>(&I)) {
                            //errs() << "ima add ili mul ili div ili sub" << "\n";
                            return true; // ne brisemo
                        }

                    }
                }
            }

            return false;
        }

        bool operandsInvariant(Loop *L) {
            for (BasicBlock *BB : LoopBodyBasicBlocks) {
                for (Instruction &I : *BB) {
                    for (Value *Op : I.operands()) {
                        if (!L->isLoopInvariant(Op)) {
                            return false;
                        }
                    }
                }
            }

            return true;
        }


        bool shouldDelete(Loop *L) {
            if(isNeverExecuted(L))
                return true;

            if(checkIfHasCall())
                return false;   // petlja ima poziv printf pa nije dead loop

            if(operandsInvariant(L)) {
                //errs() << "INVARIJANTNO OMG\n\n";
                return true;
            }



            for (const auto &entry : VariablesMap) {
                Value *InitialValue = entry.second;
                Value *ResolvedPtr = Resolve(InitialValue);

                // errs() << "Valueeeeeee: " << *InitialPtr << "\n";
                // errs() << "Mapped tooo: " << *InitialValue << "\n";
                // errs() << "Resolved to: " << *ResolvedPtr << "\n\n";

                if(!isUseful(ResolvedPtr))
                    return true;
            }

            if(hasArithmeticOperations())    // ako ovde vrati true, to znaci da ima aritmeticke operacije koje su korisne
                return false;
            
            return true;  // Vrednosti su ostale iste
        }

        

        bool DeleteLoopIfDead(Loop *L) {//false ako ne brisemo
            if(shouldDelete(L)) {
                DeleteDeadLoop(L);
                return true;
            }
                
            return false;
        }

        
        // brise petlju skroz 
        void DeleteDeadLoop(Loop *L) {

            L->getLoopPreheader()->getTerminator()->setSuccessor(0, L->getExitBlock());
            
            for(BasicBlock *BB : LoopBasicBlocks) {
                BB->eraseFromParent();
            }
        }


        bool isWhileLoop(Loop *L) {
            BasicBlock *header = L->getHeader();
            BasicBlock *latch = L->getLoopLatch();

            bool BranchingHeader = false;
            bool UncondBranchLatch = true;
            
            if (isa<BranchInst>(header->getTerminator())) {
                BranchingHeader = true;
            }

            if (BranchInst *BI = dyn_cast<BranchInst>(latch->getTerminator())) {
                UncondBranchLatch=  !BI->isConditional();
            }

            return UncondBranchLatch && BranchingHeader; 
        }


        bool runOnLoop(Loop *L, LPPassManager &LPM) override {
            VariablesMap.clear();
            FinalValues.clear();

            Loop *ParentLoop = L->getParentLoop();
    
            if (ParentLoop) {
                // errs() << "daaaaaa!\n";
                // ako brisemo roditeljsku petlju nezavisno od ove(npr ima brojac 0) onda sigurno brisemo i ovu
                if(shouldDelete(ParentLoop)) {
                    DeleteDeadLoop(L);
                    std::cout << "\n petlja JE obrisana\n";
                    return true;
                }
            } 

            LoopBodyBasicBlocks.clear();
            LoopBasicBlocks = L->getBlocksVector();

            BasicBlock *Header = L->getHeader();
            BasicBlock *Latch = L->getLoopLatch();

            findLoopCounterAndBound(L);

            for (BasicBlock *BB : LoopBasicBlocks) {
                if (BB != Header && BB != Latch) {
                    LoopBodyBasicBlocks.push_back(BB);
                }
            }

            if(LoopBodyBasicBlocks.size() < 1){
                if(isWhileLoop(L)) {
                   // errs() << "WHILE!!!\n\n";
                    LoopBodyBasicBlocks.push_back(Latch);
                    
                } else {
                    //errs() << "DO WHILE\n\n";
                    LoopBodyBasicBlocks.push_back(Header);      
                }
            }

            //printf("%d aaaaa\n", LoopBodyBasicBlocks.size());


            mapVariables();            
            saveFinalValues();

            bool res = DeleteLoopIfDead(L);
            if(res)
                std::cout << "\n petlja JE obrisana\n";
            else
                std::cout << "\n petlja NIJE obrisana\n";

            //Result == obrisana petlja promenjen IR
            return res;
        }

    };

}

char DeadLoopDeletionPass::ID = 0;
static RegisterPass<DeadLoopDeletionPass> X("dead-loops", "This pass removes loops that are useles or without side effects!");

