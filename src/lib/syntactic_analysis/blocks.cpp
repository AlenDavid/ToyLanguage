#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
// Syntax check for blocks of code.
llvm::Value *SyntaxChecker::B() {
  // For debugging purposes.
  NestLevel++;
  Debug("B()");

  // <DEFS>
  auto g = G();

  Next();

  if (CurrentToken == Token::tok_return) {
    Next();
  }

  if (CurrentToken != Token::tok_close_curly) {
    Report(";");
    return nullptr;
  }

  NestLevel--;

  return g;
}
} // namespace analysis
