#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"
#include "llvm/IR/Value.h"

using namespace tokens;

namespace analysis {
// Syntax check for blocks of code.
llvm::Value *SyntaxChecker::B() {
  // For debugging purposes.
  NestLevel++;

  // TODO: <DEFS>
  while (Next() == tokens::Token::tok_def)
    G();

  return nullptr;
}
} // namespace analysis
