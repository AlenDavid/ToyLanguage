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
    auto LeftHandler = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context),
                                              Factory.CurrentIntValue);
    Next();

    // finish expression.
    if (CurrentToken == Token::tok_end) {
      NestLevel--;
      return LeftHandler;
    }

    // TODO: handle >
    if (CurrentToken == Token::tok_greater_than) {
      NestLevel--;
      return LeftHandler;
    }

    // finish +
    if (CurrentToken == Token::tok_plus) {
      NestLevel--;
      return Builder->CreateAdd(LeftHandler, E());
    }

    if (CurrentToken != Token::tok_end) {
      return Expect(";");
    }
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

    auto var = Variables[identifier];
    if (!var)
      return Error("undefined var name");

    Debug("Builder->createLoad");

    // TODO: use Builder->CreateLoad();
    return var;
  }

  NestLevel--;
  return nullptr;
}
} // namespace analysis
