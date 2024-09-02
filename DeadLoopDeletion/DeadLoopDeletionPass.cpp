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
        void deleteDeadLoop(Loop *L, DominatorTree &DT, LoopInfo& LI, ScalarEvolution &SE)
        {
            auto *Preheader = L->getLoopPreheader();
            assert(Preheader && "Preheader should exist!");

            SE.forgetLoop(L);

            auto *ExitBlock = L->getUniqueExitBlock();
            if (!ExitBlock) {
                errs() << "Error: Loop does not have a unique exit block! Cannot proceed with the transformation.\n";
                return;
            }
            if (!L->hasDedicatedExits()) {
                errs() << "Error: Loop does not have dedicated exits! The loop might need to be simplified first.\n";
                return;
            }

            // prespajamo instrukcije tako da se terminirajuca instrukcija u preheader-u nastavlja direktno na exitblock loop-a
            Preheader->getTerminator()->replaceUsesOfWith(L->getHeader(), ExitBlock);

            BasicBlock::iterator BI = ExitBlock->begin();
            while (PHINode *P = dyn_cast<PHINode>(BI)) {
                int PredIndex = 0;
                P->setIncomingBlock(PredIndex, Preheader);

                // kad brisemo petlju, moramo da obrisemo PHI instrukcije koje su se objedinjavale iz razlicitih exit blokova da bi IR ostao validan
                // brise se unazad zbog siftovanja indeksa kada se brisu PHI cvorovi
                unsigned numIncomingValues = P->getNumIncomingValues();

                while (numIncomingValues > 1) {
                    P->removeIncomingValue(numIncomingValues - 1, false);
                    --numIncomingValues;
                }

                //PHINode treba da ima samo jednog prethodnika i incoming vrednost za PredIndex blok treba da bude Preheader
                bool hasSingleIncomingValue = (numIncomingValues == 1);
                bool hasCorrectIncomingBlock = (P->getIncomingBlock(PredIndex) == Preheader);

                if (!hasSingleIncomingValue || !hasCorrectIncomingBlock)
                    return;
                BI++;
            }

        }

        bool deleteLoopIfDead(Loop* L, DominatorTree &DT, LoopInfo& LI, ScalarEvolution &SE) {
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
            deleteDeadLoop(L, DT, LI, SE);
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

