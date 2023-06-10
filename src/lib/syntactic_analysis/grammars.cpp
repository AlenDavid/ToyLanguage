#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
// For grammar checking
llvm::Value *SyntaxChecker::T() {
  if (CurrentToken == Token::tok_def) {
    auto d = D();

    if (!d)
      return nullptr;

    return d;
  }

  return nullptr;
}

llvm::Value *SyntaxChecker::G() {
  // construct a definition
  if (CurrentToken == Token::tok_def)
    return D();

  // construct a return
  if (CurrentToken == tokens::Token::tok_return) {
    auto e = E();

    if (!e)
      return Expect("return <expression>;");

    if (CurrentToken != Token::tok_end)
      return Expect("end");

    return Builder->CreateRet(e);
  }

  return Error("unkown syntax");
}
} // namespace analysis
