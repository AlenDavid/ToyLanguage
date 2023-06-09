#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/FileSystem.h"
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
      llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), Empty, false);

  llvm::Function *TheFunction = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "main", Module.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "entry", TheFunction);

  cout << "SetInsertPoint" << endl;
  Builder->SetInsertPoint(BB);

  auto Ty = Type::getInt64Ty(ctx);

  Value *v = ConstantInt::get(Ty, 5);
  Value *v2 = ConstantInt::get(Ty, 7);

  cout << "CreateAlloca" << endl;
  auto *A = Builder->CreateAlloca(Ty, nullptr, "a");
  auto *A2 = Builder->CreateAlloca(Ty, nullptr, "a");

  cout << "CreateStore" << endl;
  Builder->CreateStore(v, A, false);
  Builder->CreateStore(v2, A2, false);

  cout << "CreateAdd" << endl;
  auto load = Builder->CreateLoad(v->getType(), v, "vload");
  auto load2 = Builder->CreateLoad(v2->getType(), v2, "v2load");

  auto add = Builder->CreateAdd(load, load2, "addtmp");

  Builder->CreateRet(add);

  std::error_code EC;
  llvm::raw_fd_ostream dest("playground.ll", EC, llvm::sys::fs::OF_None);

  Module.get()->print(dest, nullptr, true, true);

  return 0;
}
