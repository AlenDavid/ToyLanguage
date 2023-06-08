#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
// For grammar checking
llvm::Value *SyntaxChecker::G() {
  if (CurrentToken == Token::tok_def) {
    auto d = D();
    if (!d)
      Report("def");

    return d;
  }

  return nullptr;
}
} // namespace analysis
