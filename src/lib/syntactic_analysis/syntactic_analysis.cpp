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

#include "lib/code_generator/function.h"
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

// Will enable logs from the analysis.
void SyntaxChecker::EnableDebug() { _Debug = true; }

// Log message if Debug is true.
void SyntaxChecker::Debug(const std::string &message) const {
  if (_Debug) {
    for (auto i = 0; i < NestLevel; i++)
      llvm::dbgs() << "  ";
    llvm::dbgs() << message << "\n";
  }
}

std::nullptr_t SyntaxChecker::Error(const std::string &message) {
  llvm::errs() << message << " at line " << Factory.CurrentLine + 1
               << ", column " << Factory.CaretPlace + 1 << "\n";
  return nullptr;
}

std::nullptr_t SyntaxChecker::Expect(const std::string &expected) {
  llvm::errs() << "expected: " << expected << " got: ";

  if (CurrentToken == Token::tok_identifier ||
      CurrentToken == Token::tok_string) {
    llvm::errs() << Factory.CurrentIdentifier;
  }

  else if (CurrentToken == Token::tok_double)
    llvm::errs() << Factory.CurrentFloatValue;

  else if (CurrentToken == Token::tok_int)
    llvm::errs() << Factory.CurrentIntValue;

  else {
    llvm::errs() << tokens::From(CurrentToken);
  }

  llvm::errs() << " at line " << Factory.CurrentLine + 1 << ", column "
               << Factory.CaretPlace + 1 << "\n";

  return nullptr;
}

llvm::Expected<std::unique_ptr<SyntaxChecker>> SyntaxChecker::Codegen() {
  while (Next() != Token::tok_eof) {
    if (!G())
      return Error("wrong grammar");
  }

  return nullptr;
}
} // namespace analysis
