#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Value.h"

using namespace tokens;

namespace analysis {
// Syntax check for blocks of code.
// Deprecated.
llvm::Value *SyntaxChecker::B(llvm::BasicBlock *BB) {
  // For debugging purposes.
  NestLevel++;

  Builder->SetInsertPoint(BB);

  while (Next() == tokens::Token::tok_def) {
    if (!G())
      return Expect("G");
  }

  NestLevel--;
  return BB;
}

llvm::Value *SyntaxChecker::B(llvm::Function *Parent, const std::string &Name) {
  if (Next() != Token::tok_open_curly)
    Expect("{");

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(Builder->getContext(), Name, Parent);

  Builder->SetInsertPoint(BB);

  // multiple definitions, ifs, returns can exist
  while (Next() != tokens::Token::tok_close_curly) {
    auto g = G(); // consume grammar
    if (!g) {
      return nullptr;
    }
  }

  return BB;
}
} // namespace analysis
