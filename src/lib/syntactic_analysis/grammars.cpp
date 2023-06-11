#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"

using namespace tokens;

namespace analysis {
// For grammar checking
llvm::Value *SyntaxChecker::T() {
  if (CurrentToken == Token::tok_def) {
    auto d = D();

    if (!d)
      return nullptr;

    return d;
  }

  return nullptr;
}

llvm::Value *SyntaxChecker::G(llvm::Function *Parent) {
  auto Ty = llvm::Type::getInt32Ty(Context);
  auto TyVoid = llvm::Type::getVoidTy(Context);
  auto TyBool = llvm::Type::getInt1Ty(Context);

  auto CreateInt = [Ty](int V) { return llvm::ConstantInt::get(Ty, V); };
  auto CreateBool = [TyBool](int V) {
    return llvm::ConstantInt::get(TyBool, V);
  };

  llvm::Value *True = CreateBool(1);
  llvm::Value *False = CreateBool(0);

  // construct a definition
  if (CurrentToken == Token::tok_def)
    return D();

  // construct an if
  if (CurrentToken == Token::tok_if) {
    if (Next() != tokens::Token::tok_open_parenthesis)
      return Expect("(");

    auto e = E();

    if (CurrentToken != tokens::Token::tok_close_parenthesis)
      return Expect(")");

    auto currentBlock = Builder->GetInsertBlock();
    auto res = B(Parent, "if");
    if (!res)
      return nullptr;

    auto ifBlock = llvm::cast<llvm::BasicBlock>(res);

    auto afterBlock = llvm::BasicBlock::Create(Context, "after", Parent);

    Builder->SetInsertPoint(currentBlock);
    Builder->CreateCondBr(True, ifBlock, afterBlock);
    Builder->SetInsertPoint(afterBlock);
  }

  // construct a return
  if (CurrentToken == tokens::Token::tok_return) {
    auto e = E();

    if (!e)
      return Expect("return <expression>;");

    if (CurrentToken != Token::tok_end)
      return Expect(";");

    return Builder->CreateRet(e);
  }

  // this is fine; it might mean we got an empty block
  return nullptr;
}
} // namespace analysis
