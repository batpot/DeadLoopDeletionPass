#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopUnrollAnalyzer.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/LoopPeel.h"
#include "llvm/Transforms/Utils/LoopSimplify.h"
#include "llvm/Transforms/Utils/SizeOpts.h"
#include "llvm/Transforms/Utils/UnrollLoop.h"

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"

using namespace llvm;



namespace {
    struct DeadLoopDeletionPass : public LoopPass {
        static char ID;
        DeadLoopDeletionPass() : LoopPass(ID) {}


        // brisemo petlje koje 
        // - nikad se ne izvrsavaju 
        // - se izvrsavaju ali ne uticu na rezultat 
        // - nemaju bocne efekte

        //isLoopDead --> se izvrsavaju ali ne uticu na rezultat  && nemaju bocne efekte
        
        // bool isLoopNeverExecuted(Loop *L){
        //     // todo
        // }

        // bool hasSideEffects(Loop *L) {
        //     // todo 
        // }

        bool isLoopDead(Loop* L, SmallVectorImpl<BasicBlock *> &ExitingBlocks, bool &Changed, BasicBlock *ExitBlock, BasicBlock *Preheader) {
            BasicBlock::iterator BI = ExitBlock->begin();
            bool AllEntriesInvariant = true;
            bool AllOutgoingValuesSame = true;
            
            // phi node predstavlja 
            while (PHINode *P = dyn_cast<PHINode>(BI)) {
                Value *incoming = P->getIncomingValueForBlock(ExitingBlocks[0]);

                bool AllOutgoingValuesSame = true;
                for (auto It = ExitingBlocks.begin() + 1; It != ExitingBlocks.end(); ++It) {
                    if (incoming != P->getIncomingValueForBlock(*It)) {
                        // AllOutgoingValuesSame = false;
                        return false;
                    }
                }

                if (Instruction *I = dyn_cast<Instruction>(incoming))
                    if (!L->makeLoopInvariant(I, Changed, Preheader->getTerminator())) {
                        // AllEntriesInvariant = false;
                        return false;
                    }

                ++BI;
            }

            for (auto *Block : L->blocks()) {
                for (auto &I : *Block) {
                    if (I.mayHaveSideEffects()) {
                        return false;
                    }
                }
            }
            return true;
        }



        bool makesNoProgramStateChanges(Loop *L) {
            // todo
        }


        // TODO
        void deleteDeadLoop(Loop *L, DominatorTree &DT, LoopInfo& LI)
        {

        }
        

        bool deleteLoopIfDead(Loop* L, DominatorTree &DT, LoopInfo& LI) {
            assert(L->isLCSSAForm(DT) && "Expected LCSSA!");

            BasicBlock *Preheader = L->getLoopPreheader();
            if (!Preheader || !L->hasDedicatedExits())
                return false;

            // subloops TODO

            // loop mora da ima exit block
            BasicBlock *ExitBlock = L->getUniqueExitBlock();
            SmallVector<BasicBlock *, 4> ExitingBlocks;
            L->getExitingBlocks(ExitingBlocks);

            if (!ExitBlock) {
                return false; // required single exit block
            }

            // Finally, we have to check that the loop really is dead.
            bool Changed = false;
            if(!isLoopDead(L, ExitingBlocks, Changed, ExitBlock, Preheader))
                return false; // loop is not invariant, cannot delete


            // if (isLoopNeverExecuted(L) && makesNoProgramStateChanges(L) && !hasSideEffects(L)) {
            //     L->getParentLoop()->removeChildLoop(L);
            //     return true;
            // }
            deleteDeadLoop(L, DT, LI);
            return false;
        }

        bool runOnLoop(Loop *L, LPPassManager &LPM) override {
            printf("[SOON] This pass will delete dead loops!\n");

            DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
            LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

            bool Result = deleteLoopIfDead(L, DT, LI);

            // analysis?

            // Result == obrisana petlja promenjen IR
            return Result;
        }

    };

}

char DeadLoopDeletionPass::ID = 0;
static RegisterPass<DeadLoopDeletionPass> X("dead-loops", "This pass removes loops that are useles or without side effects!");

