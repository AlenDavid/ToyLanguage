#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
SyntaxChecker::SyntaxChecker(lexical::LexicalFactory &factory)
    : Module(std::make_unique<llvm::Module>("toy language", Context)),
      Builder(std::make_unique<llvm::IRBuilder<>>(Context)), Factory(factory) {}

// Consume next token from Factory and assign to CurrentToken.
Token SyntaxChecker::Next() {
  CurrentToken = Factory.NextToken();
  Debug("[" + From(CurrentToken) + "]");

  return CurrentToken;
}

llvm::Expected<std::unique_ptr<SyntaxChecker>> SyntaxChecker::Codegen() {
  while (Next() != Token::tok_eof) {
    if (!G())
      return Error("wrong grammar");
  }

  return nullptr;
}
} // namespace analysis
