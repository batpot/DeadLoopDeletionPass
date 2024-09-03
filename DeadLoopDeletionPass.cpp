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


#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IRBuilder.h"


#include <iostream>

using namespace llvm;



namespace {
    struct DeadLoopDeletionPass : public LoopPass {
        std::vector<BasicBlock *> LoopBasicBlocks;

        static char ID;
        DeadLoopDeletionPass() : LoopPass(ID) {}


        // brisemo petlje koje 
        // - nikad se ne izvrsavaju 
        // - se izvrsavaju ali ne uticu na rezultat 
        // - nemaju bocne efekte

        //isLoopDead --> se izvrsavaju ali ne uticu na rezultat  && nemaju bocne efekte


        // brisemo one petlje koje imaju u sebi load ili store (random funkcija)
        // bool isLoopDead(Loop* L) {
        //     BasicBlock *Header = L->getHeader();
        //     BasicBlock *Latch = L->getLoopLatch();
        //     SetVector<BasicBlock*> LoopBlocks;
            
        //     // Pronađi sve blokove u telu petlje osim zaglavlja i latch
        //     for (auto *Block : L->blocks()) {
        //         if (Block == Header || Block == Latch) {
        //             continue; // Preskoči zaglavlje i latch
        //         }
        //         LoopBlocks.insert(Block);
        //     }

        //     // Prolazak kroz sve blokove tela petlje
        //     for (auto *Block : LoopBlocks) {
        //         for (auto &I : *Block) {
        //             if (isa<LoadInst>(I) || isa<StoreInst>(I) || isa<CallInst>(I)) {
        //                 errs() << "Instrukcija " << I.getOpcodeName() << " se nalazi u telu petlje u bloku: " << Block->getName() << "\n";
        //                 return true;
        //             }
        //         }
        //     }

        //     return true;
        // }         

            
        
        // brise petlju skroz 
        void DeleteDeadLoop(Loop *L) {
            std::vector<BasicBlock *> LoopBodyBasicBlocks(LoopBasicBlocks.size() - 2);
            std::copy(LoopBasicBlocks.begin() + 1, LoopBasicBlocks.end() - 1, LoopBodyBasicBlocks.begin()); // [)

            L->getLoopPreheader()->getTerminator()->setSuccessor(0, L->getExitBlock());
            
            for(BasicBlock *BB : LoopBasicBlocks) {
                BB->eraseFromParent();
            }
        }


        bool deleteLoopIfDead(Loop* L) {
            BasicBlock *Preheader = L->getLoopPreheader();
            if (!Preheader || !L->hasDedicatedExits())
                return false;

            // subloops TODO

            BasicBlock *ExitBlock = L->getUniqueExitBlock();
            if (!ExitBlock) {
                return false; // required single exit block
            }

            // if(!isLoopDead(L))
            //     return false; // loop is not invariant, cannot delete

            // uvek brisemo (treba dodati uslove za brisanje, sad samo isprobavam da li se petlja uopste brise pomocu DeleteDeadLoop)
            DeleteDeadLoop(L);
            return true;
        }


        bool runOnLoop(Loop *L, LPPassManager &LPM) override {
            //printf("[SOON] This pass will delete dead loops!\n");            

            LoopBasicBlocks = L->getBlocksVector();
            // printf("eeeeeeeeee: %d\n", (int)LoopBasicBlocks.size());

            // Result == obrisana petlja promenjen IR
            bool Result = deleteLoopIfDead(L);
            return Result;
        }

    };

}

char DeadLoopDeletionPass::ID = 0;
static RegisterPass<DeadLoopDeletionPass> X("dead-loops", "This pass removes loops that are useles or without side effects!");

