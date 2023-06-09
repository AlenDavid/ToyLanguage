#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include <iostream>

int run_pass_on_module(llvm::Module *Module, const char *Filename) {

  if (Filename == nullptr) {
    Filename = "output.o";
  }

  auto CPU = "generic";
  auto Features = "";
  std::string Error;

  std::cout << "Before getDefaultTargetTriple" << std::endl;
  auto TargetTriple = llvm::sys::getDefaultTargetTriple();

  std::cout << "Before TargetRegistry::lookupTarget" << std::endl;
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
  auto TargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  std::cout << "Before Data Layout" << std::endl;
  Module->setDataLayout(TargetMachine->createDataLayout());
  std::cout << "Before Target Triple" << std::endl;
  Module->setTargetTriple(TargetTriple);

  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }

  // create object file
  llvm::legacy::PassManager pass;

  if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr,
                                         llvm::CGFT_ObjectFile)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }
  std::cout << "Before Pass" << std::endl;

  if (!pass.run(*Module)) {
    llvm::errs() << "couldn't pass module";
  }
  dest.flush();

  return 0;
}
