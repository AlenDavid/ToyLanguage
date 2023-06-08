#include "function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include <iostream>
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

namespace nodes {
FunctionAST::FunctionAST(const std::string &Name, std::vector<std::string> Args,
                         llvm::Value *Block, llvm::Value *ReturnValue)
    : Name(Name), Args(std::move(Args)), Block(std::move(Block)),
      ReturnValue(std::move(ReturnValue)) {}

const std::string &FunctionAST::getName() const { return Name; }

llvm::Function *FunctionAST::codegen(llvm::Module *Module,
                                     llvm::IRBuilder<> *Builder) {
  // TODO: handle parameters.
  std::vector<llvm::Type *> Empty(0);

  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getInt32Ty(Module->getContext()), Empty, false);

  llvm::Function *TheFunction = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, getName(), *Module);

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(Module->getContext(), "entry", TheFunction);

  Builder->SetInsertPoint(BB);

  Builder->CreateRet(ReturnValue);

  return TheFunction;
}
} // namespace nodes
