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
  NestLevel++;
  Next();

  if (CurrentToken == Token::tok_open_parenthesis) {
    auto e = E();
    Next();

    if (CurrentToken != Token::tok_close_parenthesis) {
      return Expect(")");
    }

    return e;
  }

  if (CurrentToken == Token::tok_double) {
    auto value = Factory.CurrentIntValue;

    Debug("Token: " + From(CurrentToken) + " \"" + std::to_string(value) +
          "\"");

    Next();

    if (CurrentToken == Token::tok_end) {
      NestLevel--;
      return llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), value);
    }

    if (CurrentToken == Token::tok_greater_than) {
      return E();
    }

    Expect(";");
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
      Expect("\"");
      NestLevel--;
      return nullptr;
    }

    Next();

    if (CurrentToken != Token::tok_end) {
      Expect(";");
      NestLevel--;
      return nullptr;
    }

    NestLevel--;
    return Builder->CreateGlobalStringPtr(llvm::StringRef(text));
  }

  // <var>
  if (CurrentToken == tokens::Token::tok_identifier) {
    auto identifier = Factory.CurrentIdentifier;
    // consume identifier
    Next();

    return Variables[identifier];
  }

  NestLevel--;
  return nullptr;
}
} // namespace analysis
