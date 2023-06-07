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

const std::string &FunctionAST::getName() const { return Name; }

llvm::Function *FunctionAST::codegen(llvm::Module *Module) {
  auto Builder = llvm::IRBuilder(Module->getContext());

  // TODO: handle parameters.
  std::vector<llvm::Type *> Empty(0);

  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getInt32Ty(Module->getContext()), Empty, false);

  // eat identifier
  llvm::Function *TheFunction = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, getName(), *Module);

  // eat {
  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(Module->getContext(), "block", TheFunction);

  // eat G()
  Builder.SetInsertPoint(BB);

  // eat return
  if (llvm::Value *RetVal = llvm::ConstantInt::get(
          llvm::Type::getInt32Ty(Module->getContext()), 5)) {

    // Finish off the function.
    Builder.CreateRet(RetVal);
  }

  return TheFunction;
}
} // namespace nodes
