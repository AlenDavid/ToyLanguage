#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "lib/code_generator/function.h"
#include "lib/code_generator/variable.h"
#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis {
// Syntax check for definitions.
llvm::Value *SyntaxChecker::D() {
  NestLevel++;

  // consume ID.
  Next();
  Debug("Token: " + From(CurrentToken) + " " + Factory.CurrentIdentifier);
  auto name = Factory.CurrentIdentifier;

  if (CurrentToken != Token::tok_identifier) {
    Report("name");
    NestLevel--;

    return nullptr;
  }

  Next();

  // <DEF> <ID> = <E>
  if (CurrentToken == Token::tok_equal) {
    NestLevel--;
    auto e = E();

    if (!e) {
      Report("expression");
      return nullptr;
    }

    return nodes::VariableAST(name, e).codegen(Builder.get());
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
      Report(")");
      NestLevel--;
      return nullptr;
    }

    if (Next() != Token::tok_open_curly) {
      Report("{");
      NestLevel--;
      return nullptr;
    }

    llvm::FunctionType *FT = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(Module->getContext()), Empty, false);

    llvm::Function *TheFunction = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *Module);

    llvm::BasicBlock *BB =
        llvm::BasicBlock::Create(Module->getContext(), "entry", TheFunction);

    Builder->SetInsertPoint(BB);

    auto block = B();

    if (CurrentToken != tokens::Token::tok_return) {
      Report("return");
      NestLevel--;
      return nullptr;
    }

    auto e = E();

    if (!e) {
      Report("expression");
      NestLevel--;
      ;
      return nullptr;
    }

    if (CurrentToken != Token::tok_end) {
      Report("end");
      NestLevel--;
      return nullptr;
    }

    if (Next() != Token::tok_close_curly) {
      Report("}");
      NestLevel--;
      return nullptr;
    }

    NestLevel--;
    return Builder->CreateRet(e);
  }

  Report("Unkown");
  NestLevel--;
  return nullptr;
}
} // namespace analysis
