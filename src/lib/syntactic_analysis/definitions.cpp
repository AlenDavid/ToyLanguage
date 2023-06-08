#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "lib/code_generator/function.h"
#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
// Syntax check for definitions.
llvm::Value *SyntaxChecker::D() {
  NestLevel++;

  // consume ID.
  Next();
  Debug("Token: " + From(CurrentToken) + " " + Factory.CurrentIdentifier);

  if (CurrentToken != Token::tok_identifier) {
    Report("name");
    NestLevel++;

    return nullptr;
  }

  Next();

  // <DEF> <ID> = <E>
  if (CurrentToken == Token::tok_equal) {
    NestLevel--;
    return E();
  }

  // <DEF> <ID> <PARAMS> <BLOCK>
  // <PARAMS> |== (<PARAM>)
  if (CurrentToken == Token::tok_open_parenthesis) {
    auto fnName = Factory.CurrentIdentifier;
    auto fnArgs = std::vector<std::string>();

    while (Token::tok_identifier == Next()) {
      fnArgs.push_back(Factory.CurrentIdentifier);
    }

    if (CurrentToken != Token::tok_close_parenthesis) {
      Report(")");
      NestLevel--;
      return nullptr;
    }

    if (Next() != Token::tok_open_curly) {
      Report("{");
      NestLevel--;
      return nullptr;
    }

    auto block = B();

    auto fn = nodes::FunctionAST(fnName, fnArgs, block, nullptr);
    return fn.codegen(Module.get());
  }

  Report("Unkown");
  NestLevel--;
  return nullptr;
}
} // namespace analysis
