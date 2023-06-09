#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/TargetSelect.h"
#include <iostream>
#include <memory>
#include <string>

#include "lib/module_emmiter/module_emmiter.h"

using namespace std;
using namespace llvm;

int main() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto ctx = llvm::LLVMContext();

  unique_ptr<llvm::Module> Module;
  unique_ptr<llvm::IRBuilder<>> Builder;

  Module = make_unique<llvm::Module>("playground", ctx);
  Builder = make_unique<llvm::IRBuilder<>>(ctx);

  std::vector<llvm::Type *> Empty(0);

  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx), Empty, false);

  llvm::Function *TheFunction = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "main", Module.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "entry", TheFunction);

  cout << "SetInsertPoint" << endl;
  Builder->SetInsertPoint(BB);
  Value *v = ConstantInt::get(Type::getInt32Ty(ctx), 5);

  cout << "CreateAlloca" << endl;
  auto *A = Builder->CreateAlloca(v->getType(), nullptr, "a");

  cout << "CreateStore" << endl;
  auto store = Builder->CreateStore(v, A, false);

  Builder->CreateRet(v);

  cout << "Pass" << endl;

  if (Module.get()) {
    cout << Module.get()->getName().str() << endl;
  }

  run_pass_on_module(Module.get(), "playground.o");

  return 0;
}
