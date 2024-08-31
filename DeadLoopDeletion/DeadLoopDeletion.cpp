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

using namespace llvm;

namespace {
    struct DeadLoopDeletionPass : public LoopPass {
        static char ID;
        DeadLoopDeletionPass() : LoopPass(ID) {}

        bool runOnLoop(Loop *L, LPPassManager &LPM) override {
            printf("[SOON] This pass will delete dead loops!\n");
            return false;
        }
    };
}

char DeadLoopDeletionPass::ID = 0;
static RegisterPass<DeadLoopDeletionPass> X("dead-loops", "This pass removes loops that are useles or without side effects!");

