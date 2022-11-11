#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct CountPass : public FunctionPass {
  static char ID;
  CountPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    outs() << F.getName() << ':'<<F.getBasicBlockList().size() << '\n';
    return false;
  }
};
} // namespace

char CountPass::ID = 0;
static RegisterPass<CountPass> X("CountPass", "Count Pass", false, false);