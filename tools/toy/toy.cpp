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
#include "../../src/lib/syntax_checker/syntax_checker.hh"

int main(int argc, char **argv)
{
  std::cout << "Toy Language compiler" << std::endl;

  auto code = parse_args(argc, argv);

  if (code == "")
  {
    return 1;
  }

  auto parser = SyntaxChecker(code);

  while (parser.GetToken() != tok_eof)
    ;

  std::cout << "Tokens:\n\n";
  for (auto i : parser.Tokens)
  {
    std::cout << i << std::endl;
  }

  std::cout << "Identifiers:\n\n";
  for (auto i : parser.Identifiers)
  {
    std::cout << i << std::endl;
  }
}
