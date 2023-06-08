#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include <iostream>
#include <memory>
#include <string>

using namespace llvm;

int main() {
  auto ctx = llvm::LLVMContext();

  llvm::Module Module("playground", ctx);
  IRBuilder<> Builder(ctx);

  std::vector<llvm::Type *> Empty(0);

  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx), Empty, false);

  llvm::Function *TheFunction = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "__entry", Module);

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "entry", TheFunction);

  Builder.SetInsertPoint(BB);

  return 0;
}
