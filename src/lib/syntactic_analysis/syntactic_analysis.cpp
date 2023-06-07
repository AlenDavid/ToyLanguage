#include "llvm/IR/Module.h"
#include "llvm/Support/Error.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "lib/code_generator/function.h"
#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
SyntaxChecker::SyntaxChecker(lexical::LexicalFactory &factory)
    : Module(std::make_unique<llvm::Module>("toy language", Context)),
      Factory(factory) {}

// Consume next token from Factory and assign to CurrentToken.
Token SyntaxChecker::Next() {
  CurrentToken = Factory.NextToken();
  Debug("[" + From(CurrentToken) + "]");

  return CurrentToken;
}

// Will enable logs from the analysis.
void SyntaxChecker::EnableDebug() { _Debug = true; }

// Log message if Debug is true.
void SyntaxChecker::Debug(const std::string &message) const {
  if (_Debug) {
    for (auto i = 0; i < NestLevel; i++)
      std::cout << "  ";
    std::cout << message << std::endl;
  }
}

// Store analysis errors.
void SyntaxChecker::Report(const std::string &expected) {
  std::ostringstream oss;

  oss << "expected: " << expected << " got: ";

  if (CurrentToken == Token::tok_identifier ||
      CurrentToken == Token::tok_string) {
    oss << Factory.CurrentIdentifier;
  }

  else if (CurrentToken == Token::tok_double ||
           CurrentToken == Token::tok_int) {
    oss << Factory.CurrentNumericValue;
  }

  else {
    oss << tokens::From(CurrentToken);
  }

  oss << " at line " << Factory.CurrentLine + 1 << ", column "
      << Factory.CaretPlace + 1;

  Debug(oss.str());
  Errs.emplace_back(oss.str());

  oss.clear();
}

llvm::Expected<std::unique_ptr<SyntaxChecker>> SyntaxChecker::Codegen() {
  auto fn = nodes::FunctionAST("main", std::vector<std::string>());
  fn.codegen(Module.get());

  return nullptr;
}
} // namespace analysis
