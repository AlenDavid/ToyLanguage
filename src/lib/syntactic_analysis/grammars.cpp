#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
// For grammar checking
bool SyntaxChecker::G() {
  NestLevel++;
  Debug("G()");

  for (Next(); CurrentToken != Token::tok_eof; Next()) {
    if (CurrentToken == Token::tok_def && !D())
      return false;

    // tok_identifier | tok_return
    if (std::find(ExpressionTokens.begin(), ExpressionTokens.end(),
                  CurrentToken) != ExpressionTokens.end()) {
      auto e = E();

      if (!e) {
        Report("expression");
      }

      return e;
    }
  }

  NestLevel--;

  return true;
}
} // namespace analysis
