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

  // special case for unary - <E>;
  if (CurrentToken == Token::tok_minus) {
    auto LeftHandler = E();
    NestLevel--;

    return Builder->CreateMul(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), -1),
        LeftHandler);
  }

  if (CurrentToken == Token::tok_double) {
    auto LeftHandler = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context),
                                              Factory.CurrentIntValue);
    NestLevel--;
    Next();

    // finish expression.
    if (CurrentToken == Token::tok_end ||
        CurrentToken == tokens::Token::tok_close_parenthesis)
      return LeftHandler;

    // finish <
    if (CurrentToken == Token::tok_smaller_than) {
      auto L =
          Builder->CreateUIToFP(LeftHandler, llvm::Type::getDoubleTy(Context));
      auto R = Builder->CreateUIToFP(E(), llvm::Type::getDoubleTy(Context));

      return Builder->CreateFCmpULT(L, R, "comp");
    }

    // finish >
    if (CurrentToken == Token::tok_greater_than) {
      auto L =
          Builder->CreateUIToFP(LeftHandler, llvm::Type::getDoubleTy(Context));
      auto R = Builder->CreateUIToFP(E(), llvm::Type::getDoubleTy(Context));

      return Builder->CreateFCmpUGT(L, R, "comp");
    }

    // finish +
    if (CurrentToken == Token::tok_plus)
      return Builder->CreateAdd(LeftHandler, E());

    // finish -
    if (CurrentToken == Token::tok_minus)
      return Builder->CreateSub(LeftHandler, E());

    // finish *
    if (CurrentToken == Token::tok_times)
      return Builder->CreateMul(LeftHandler, E());

    // finish /
    if (CurrentToken == Token::tok_divider)
      return Builder->CreateFDiv(LeftHandler, E());
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
