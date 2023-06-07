#include <iostream>
#include <string>

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

#include "lib/input_parser/input_parser.h"
#include "lib/lexical/lexical.h"
#include "lib/messages/messages.h"
#include "lib/module_emmiter/module_emmiter.h"
#include "lib/syntactic_analysis/syntactic_analysis.h"

using namespace analysis;
using namespace lexical;
using namespace tokens;

int main(int argc, char **argv) {
  bool debugging = false;

  if (argc <= 2) {
    std::cout << Messages::USAGE << std::endl;
    return 0;
  }

  // check for options.
  for (int i = 3; i < argc; i++) {
    if (std::strcmp(argv[i], "--help") == 0) {
      std::cout << Messages::USAGE << std::endl;
      return 0;
    }

    if (std::strcmp(argv[i], "--verbose") == 0)
      debugging = true;
  }

  const auto code = file_contents(argv[1]);

  if (code == "") {
    std::cout << Messages::USAGE << std::endl;
    return 1;
  }

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto factory = LexicalFactory(code);
  auto syntax = SyntaxChecker(factory);
  llvm::ExitOnError ExitOnError;

  if (debugging)
    syntax.EnableDebug();

  ExitOnError(syntax.Codegen());

  if (syntax.Errs.empty()) {
    std::cout << "Compiling code..." << std::endl;

    run_pass_on_module(syntax.Module.get(), argv[2]);
  }

  if (!syntax.Errs.empty())
    std::cout << "\nErrors:\n";

  for (const auto &err : syntax.Errs) {
    std::cout << err << std::endl;
  }

  return (int)syntax.Errs.size();
}
