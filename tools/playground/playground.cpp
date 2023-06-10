#include "llvm/ADT/APInt.h"
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
  auto Ty = Type::getInt32Ty(ctx);
  auto TyBool = Type::getInt1Ty(ctx);

  std::vector<llvm::Type *> Empty(0);
  llvm::FunctionType *FT = llvm::FunctionType::get(Ty, Empty, false);
  llvm::Function *TheFunction = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "main", Module.get());
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "entry", TheFunction);

  Builder->SetInsertPoint(BB);

  auto CreateInt = [Ty](int V) { return ConstantInt::get(Ty, V); };
  auto CreateBool = [TyBool](int V) { return ConstantInt::get(TyBool, V); };

  Value *True = CreateBool(1);
  Value *False = CreateBool(0);

  auto *A = Builder->CreateAlloca(Ty, nullptr, "a");

  llvm::BasicBlock *Ablablueble =
      llvm::BasicBlock::Create(ctx, "ablabluble", TheFunction);
  llvm::BasicBlock *Then = llvm::BasicBlock::Create(ctx, "then", TheFunction);
  llvm::BasicBlock *After = llvm::BasicBlock::Create(ctx, "after", TheFunction);

  // Builder->CreateCondBr(True, Ablablueble, Ablablueble);
  Builder->CreateBr(Ablablueble);

  Builder->SetInsertPoint(Ablablueble);
  Builder->CreateRet(CreateInt(1));

  Builder->SetInsertPoint(Then);
  Builder->CreateRet(CreateInt(2));

  Builder->SetInsertPoint(After);

  Builder->CreateStore(CreateInt(3), A, false);
  auto load = Builder->CreateLoad(Ty, A, "vload");

  Builder->CreateRet(load);

  std::error_code EC;
  llvm::raw_fd_ostream dest("playground.ll", EC, llvm::sys::fs::OF_None);

  Module.get()->print(dest, nullptr, true, true);

  return run_pass_on_module(Module.get(), "playground.o");
}
