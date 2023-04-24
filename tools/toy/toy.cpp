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

#include "../../src/lib/input_parser.hh"

static std::unique_ptr<llvm::LLVMContext> TheContext;
static std::unique_ptr<llvm::Module> TheModule;
static std::unique_ptr<llvm::IRBuilder<>> Builder;
static llvm::ExitOnError ExitOnErr;

static void InitializeModuleAndPassManager() {
  // Open a new module.
  TheContext = std::make_unique<llvm::LLVMContext>();
  TheModule = std::make_unique<llvm::Module>("toy language", *TheContext);

  // Create a new builder for the module.
  Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
}


llvm::Value* generateDouble(double num) {
  return llvm::ConstantFP::get(*TheContext, llvm::APFloat(num));
}

int main(int argc,  char** argv)
{
  std::cout << "Toy Language compiler" << std::endl;

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  InitializeModuleAndPassManager();

  // Make the function type:  double(double,double) etc.
  std::vector<llvm::Type *> Doubles(1, llvm::Type::getDoubleTy(*TheContext));
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);

  llvm::Function *TheFunction =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "main", TheModule.get());
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);

  Builder->SetInsertPoint(BB);

  if (llvm::Value *RetVal = llvm::ConstantFP::get(*TheContext, llvm::APFloat(5.0))) {
    // Finish off the function.
    Builder->CreateRet(RetVal);
  }

  auto n = generateDouble(5);

  llvm::errs() << *TheFunction;
}
