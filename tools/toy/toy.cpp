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

#include "../../src/lib/input_parser/input_parser.h"
#include "../../src/lib/lexical/lexical.h"
#include "../../src/lib/syntactic_analysis/syntactic_analysis.h"

using namespace tokens;
using namespace lexical;
using namespace analysis;

int main(int argc, char **argv)
{
  bool debugging = false;

  for (int i = 0; i < argc; i++)
  {
    if (std::strcmp(argv[i], "--debug"))
      debugging = true;
  }

  std::cout << "Toy Language compiler" << std::endl;

  const auto code = parse_args(argc, argv);

  if (code == "")
    return 1;

  auto factory = LexicalFactory(code);
  auto checker = SyntaxChecker(factory);

  if (debugging)
    checker.EnableDebug();
  checker.G();

  if (!checker.Errs.empty())
    std::cout << "\nErrors:\n";

  for (const auto &err : checker.Errs)
  {
    std::cout << err << std::endl;
  }

  return (int)checker.Errs.size();
}
