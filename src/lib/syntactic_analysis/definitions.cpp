#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "lib/code_generator/function.h"
#include "lib/code_generator/variable.h"
#include "syntactic_analysis.h"
#include "llvm/IR/BasicBlock.h"

using namespace tokens;

namespace analysis {
// Syntax check for definitions.
llvm::Value *SyntaxChecker::D() {
  NestLevel++;

  // consume ID.
  Next();

  auto name = Factory.CurrentIdentifier;

  if (CurrentToken != Token::tok_identifier)
    return Expect("name");

  Next();

  // <DEF> <ID> = <E>
  if (CurrentToken == Token::tok_equal) {
    auto e = E();

    if (!e)
      return Expect("expression");

    NestLevel--;
    auto variable = nodes::VariableAST(name, e).codegen(Builder.get());
    Variables[name] = Builder->CreateLoad(e->getType(), variable, name);

    return variable;
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

    if (Next() != Token::tok_open_curly)
      Expect("{");

    llvm::FunctionType *FT = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(Module->getContext()), Empty, false);

    llvm::Function *TheFunction = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *Module);

    // Block can be null for the sake of just a return exp.
    auto Block = llvm::cast<llvm::BasicBlock>(B(TheFunction));

    if (CurrentToken != tokens::Token::tok_return)
      return Expect("return");

    auto e = E();

    if (!e)
      return Expect("expression");

    Builder->CreateRet(e);

    if (CurrentToken != Token::tok_end)
      return Expect("end");

    if (Next() != Token::tok_close_curly)
      return Expect("}");

    NestLevel--;

    return TheFunction;
  }

  return Expect("Unkown");
}
} // namespace analysis
