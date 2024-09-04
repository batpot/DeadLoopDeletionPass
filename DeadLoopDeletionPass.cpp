#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopUnrollAnalyzer.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/LoopPeel.h"
#include "llvm/Transforms/Utils/LoopSimplify.h"
#include "llvm/Transforms/Utils/SizeOpts.h"
#include "llvm/Transforms/Utils/UnrollLoop.h"
#include "llvm/ADT/Statistic.h"


#include <iostream>

using namespace llvm;

namespace {
    struct DeadLoopDeletionPass : public LoopPass {
        std::vector<BasicBlock *> LoopBasicBlocks;
        std::unordered_map<Value *, Value *> VariablesMap;
    	Value *LoopCounter, *LoopBound;
    	bool isLoopBoundConst;
    	int BoundValue;

        static char ID;
        DeadLoopDeletionPass() : LoopPass(ID) {}
        
        void mapVariables(Loop *L)
  		{
    		Function *F = L->getHeader()->getParent();
    		for (BasicBlock &BB : *F) {
      			for (Instruction &I : BB) {
        			if (isa<LoadInst>(&I)) {
          				VariablesMap[&I] = I.getOperand(0);
        			}
      			}
    		}
  		}

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
        
        // PROVERA ZA LOOP KOJI SE NIKAD NE IZVRSAVA
        
        void findLoopCounterAndBound(Loop *L)
        {
        	for (Instruction &I : *L->getHeader()) {
        		if (isa<ICmpInst>(&I)) {
        			LoopCounter = VariablesMap[I.getOperand(0)];
        			if (ConstantInt *ConstInt = dyn_cast<ConstantInt>(I.getOperand(1))) {
        				isLoopBoundConst = true;
        				BoundValue = ConstInt->getSExtValue();
        			}
        		}
        	}
        }
        
        bool isNeverExecuted(Loop *L)
        {
        	// TODO uopsti da ceo uslov bude neki false condition kao sto je i<0, mozda cak i na manje od 0??
			if(BoundValue == 0)
           	{
            	//errs() << "isNeverExecuted: true" << "\n";
           		return true;
            }
            else
            {
            	//errs() << "isNeverExecuted: false, BoundValue=" << BoundValue << "\n";
              	return false;
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

            if(isNeverExecuted(L))
            {
            	//errs() << "loop u f-ji: " << L->getHeader()->getParent()->getName().str()	<< " se nikad ne izvrsava" << "\n";
            	DeleteDeadLoop(L);
            	return true;
            }

			return false;
        }


        bool runOnLoop(Loop *L, LPPassManager &LPM) override {
            //printf("[SOON] This pass will delete dead loops!\n");  
            mapVariables(L); 
            findLoopCounterAndBound(L);         

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

