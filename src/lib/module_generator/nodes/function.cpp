#include "function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

namespace nodes {
FunctionAST::FunctionAST(const std::string &Name, std::vector<std::string> Args)
    : Name(Name), Args(std::move(Args)) {}

const std::string &FunctionAST::getName() { return Name; }

llvm::Function *FunctionAST::codegen(llvm::Module *Module) {
  auto Builder = llvm::IRBuilder(Module->getContext());

  std::vector<llvm::Type *> Empty(0);

  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getInt32Ty(Module->getContext()), Empty, false);

  llvm::Function *TheFunction = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "main", *Module);

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(Module->getContext(), "entry", TheFunction);

  Builder.SetInsertPoint(BB);

  if (llvm::Value *RetVal = llvm::ConstantInt::get(
          llvm::Type::getInt32Ty(Module->getContext()), 27)) {

    // Finish off the function.
    Builder.CreateRet(RetVal);
  }

  return TheFunction;
}
} // namespace nodes
