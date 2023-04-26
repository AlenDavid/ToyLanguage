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

int main(int argc,  char** argv)
{
  auto CPU = "generic";
  auto Features = "";

  std::string Error;
  std::cout << "Toy Language compiler" << std::endl;

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  InitializeModuleAndPassManager();

  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    llvm::errs() << Error;
    return 1;
  }

  llvm::TargetOptions opt;
  auto RM = std::optional<llvm::Reloc::Model>();
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  TheModule->setDataLayout(TargetMachine->createDataLayout());
  TheModule->setTargetTriple(TargetTriple);

  auto Filename = "output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }

  // Make the function type:  double(double,double) etc.
  std::vector<llvm::Type *> Doubles(1, llvm::Type::getDoubleTy(*TheContext));
  std::vector<llvm::Type *> Empty(0);
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), Empty, false);

  llvm::Function *TheFunction =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "main", TheModule.get());
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);

  Builder->SetInsertPoint(BB);

  if (llvm::Value *RetVal = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 5)) {
    // Finish off the function.
    Builder->CreateRet(RetVal);
  }

  llvm::legacy::PassManager pass;

  if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_ObjectFile)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*TheModule);
  dest.flush();
}
