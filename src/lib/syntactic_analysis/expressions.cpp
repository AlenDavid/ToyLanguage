#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"

using namespace tokens;

namespace analysis {
// For expressions
llvm::Value *SyntaxChecker::E() {
  auto Builder = llvm::IRBuilder(Module->getContext());

  NestLevel++;
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
    auto value = Factory.CurrentIntValue;

    Next();

    if (CurrentToken == Token::tok_end) {
      NestLevel--;
      Debug("value is: " + std::to_string(value));
      return llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), value);
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
    auto text = Factory.CurrentIdentifier;
    Debug("Token: " + From(CurrentToken) + " \"" + text + "\"");

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
    return Builder.CreateGlobalStringPtr(llvm::StringRef(text));
  }

  return nullptr;
}
} // namespace analysis
