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
    E();
    Next();

    if (CurrentToken != Token::tok_close_parenthesis) {
      Expect(")");
      NestLevel--;

      return nullptr;
    }
  }

  if (CurrentToken == Token::tok_double) {
    auto value = Factory.CurrentIntValue;

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

  NestLevel--;
  return nullptr;
}
} // namespace analysis
