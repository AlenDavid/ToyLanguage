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

  auto Context = llvm::LLVMContext();
  auto Builder = llvm::IRBuilder(Context);

  std::vector<llvm::Type *> Empty(0);

  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), Empty, false);

  llvm::Function *TheFunction =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, Module);

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(Context, "entry", TheFunction);

  Builder.SetInsertPoint(BB);

  if (llvm::Value *RetVal =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 27)) {

    // Finish off the function.
    Builder.CreateRet(RetVal);

    return TheFunction;
  }
}
} // namespace nodes
