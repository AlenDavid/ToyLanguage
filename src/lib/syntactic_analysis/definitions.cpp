#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"

using namespace tokens;

namespace analysis {
// Syntax check for definitions.
llvm::Value *SyntaxChecker::D() {
  NestLevel++;

  // consume ID.
  Next();

  if (CurrentToken != Token::tok_identifier)
    return Expect("name");

  auto name = Factory.CurrentIdentifier;

  // consume = or (
  Next();

  // <DEF> <ID> = <E>
  if (CurrentToken == Token::tok_equal) {
    auto e = E();

    if (!e)
      return Expect("def <id> = expression;");

    NestLevel--;

    auto *A = Builder->CreateAlloca(e->getType(), nullptr, name);
    auto store = Builder->CreateStore(e, A, false);

    Variables[name] = Builder->CreateLoad(e->getType(), A, name);
    return A;
  }

  // <DEF> <ID> <PARAMS> <BLOCK>
  // <PARAMS> |== (<PARAM>)
  if (CurrentToken == Token::tok_open_parenthesis) {
    auto fnArgs = std::vector<std::string>();
    // TODO: handle parameters.
    while (Token::tok_identifier == Next()) {
      fnArgs.push_back(Factory.CurrentIdentifier);
    }

    std::vector<llvm::Type *> Empty(0);

    if (CurrentToken != Token::tok_close_parenthesis) {
      Expect(")");
      NestLevel--;
      return nullptr;
    }

    // now we know its a function.

    llvm::FunctionType *FT = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(Module->getContext()), Empty, false);

    llvm::Function *TheFunction = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *Module);

    // Blocks can be empty.
    auto b = B(TheFunction);
    if (!b)
      return nullptr;

    NestLevel--;

    return TheFunction;
  }

  return Expect("Unkown");
}
} // namespace analysis
