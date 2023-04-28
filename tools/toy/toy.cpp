#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"

#include <string>
#include <iostream>
#include <fstream>

#include "../../src/lib/input_parser/input_parser.hh"
#include "../../src/lib/module_emmiter/module_emmiter.hh"

int main(int argc,  char** argv)
{
  std::cout << "Toy Language compiler" << std::endl;

  auto Context = llvm::LLVMContext();
  auto Module = llvm::Module("toy language", Context);
  auto Builder = llvm::IRBuilder(Context);
  llvm::ExitOnError ExitOnErr;

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::vector<llvm::Type *> Empty(0);
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), Empty, false);

  llvm::Function *TheFunction =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "main", Module);
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(Context, "entry", TheFunction);

  Builder.SetInsertPoint(BB);

  if (llvm::Value *RetVal = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 5)) {
    // Finish off the function.
    Builder.CreateRet(RetVal);
  }

  run_pass_on_module(&Module);
}
