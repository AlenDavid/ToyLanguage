#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
// For expressions
llvm::Value *SyntaxChecker::E() {
  NestLevel++;
  Debug("E()");
  Next();

  if (CurrentToken == Token::tok_open_parenthesis) {
    E();
    Next();

    if (CurrentToken != Token::tok_close_parenthesis) {
      Report(")");
      NestLevel--;

      return nullptr;
    }
  }

  if (CurrentToken == Token::tok_double) {
    Next();

    if (CurrentToken == Token::tok_end) {
      NestLevel--;
      // return true;
      return nullptr;
    }

    if (CurrentToken == Token::tok_greater_than) {
      return E();
    }

    Report(";");
    return nullptr;
  }

  // "[\w\W]+"
  if (CurrentToken == Token::tok_string) {
    // consume text
    Next();
    Debug("Token: " + From(CurrentToken) + " \"" + Factory.CurrentIdentifier +
          "\"");

    // consume "
    Next();

    if (CurrentToken != Token::tok_string) {
      Report("\"");
      NestLevel--;
      return nullptr;
    }

    Next();

    if (CurrentToken != Token::tok_end) {
      Report(";");
      NestLevel--;
      NestLevel--;
      return nullptr;
    }

    NestLevel--;
    // return true;
    return nullptr;
  }

  return nullptr;
}
} // namespace analysis
