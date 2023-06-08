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

  if (CurrentToken == tokens::Token::tok_return) {
    auto e = E();

    if (!e) {
      Report("expression");
      return nullptr;
    }

    if (CurrentToken != Token::tok_end) {
      Report("end");
      return nullptr;
    }

    return e;
  }

  return nullptr;
}
} // namespace analysis
