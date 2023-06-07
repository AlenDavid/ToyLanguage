#include "number.h"
#include "llvm/IR/Constants.h"
#include <llvm/IR/Value.h>

namespace nodes {
llvm::Value *NumberExprAST::codegen() {
  // TODO: be sure we want this thing here?
  auto Context = llvm::LLVMContext();

  return llvm::ConstantFP::get(Context, llvm::APFloat(Val));
}

} // namespace nodes
