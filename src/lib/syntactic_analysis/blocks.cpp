#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Casting.h"

using namespace tokens;

namespace analysis {
llvm::Value *SyntaxChecker::B(llvm::Function *Parent, const std::string &Name) {
  auto foundReturn = false;
  if (Next() != Token::tok_open_curly)
    Expect("{");

  NestLevel++;

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(Builder->getContext(), Name, Parent);

  Builder->SetInsertPoint(BB);

  // consume {
  Next();

  // multiple definitions, ifs, returns can exist
  while (CurrentToken != tokens::Token::tok_close_curly &&
         CurrentToken != tokens::Token::tok_eof) {
    auto grammar = G(Parent); // consume grammar

    if (Errored)
      break;

    // LLVM don't accept terminators in the middle of a block
    // so we must break the while loop.
    if (grammar && llvm::isa<llvm::ReturnInst>(grammar)) {
      break;
    }

    Next();
  }

  if (Next() != Token::tok_close_curly)
    return Expect("}");

  NestLevel--;
  return BB;
}
} // namespace analysis
