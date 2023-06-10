#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
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
  NestLevel--;
  llvm::errs() << message << " at line " << Factory.CurrentLine + 1
               << ", column " << Factory.CaretPlace + 1 << "\n";
  return nullptr;
}

std::nullptr_t SyntaxChecker::Expect(const std::string &expected) {
  NestLevel--;
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
} // namespace analysis
