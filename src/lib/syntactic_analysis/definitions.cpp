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
  auto name = Factory.CurrentIdentifier;

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

    if (CurrentToken != tokens::Token::tok_return) {
      Report("return");
      return nullptr;
    }
    auto e = E();

    if (!e) {
      Report("expression");
      return nullptr;
    }

    if (CurrentToken != Token::tok_end) {
      Report("end");
      return nullptr;
    }

    if (Next() != Token::tok_close_curly) {
      Report("}");
      return nullptr;
    }

    NestLevel--;

    auto fn = nodes::FunctionAST(name, fnArgs, block, e);
    return fn.codegen(Module.get(), Builder.get());
  }

  Report("Unkown");
  NestLevel--;
  return nullptr;
}
} // namespace analysis
