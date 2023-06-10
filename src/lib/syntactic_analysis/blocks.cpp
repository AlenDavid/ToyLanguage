#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "syntactic_analysis.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Casting.h"

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
  auto foundReturn = false;
  if (Next() != Token::tok_open_curly)
    Expect("{");

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(Builder->getContext(), Name, Parent);

  Builder->SetInsertPoint(BB);

  // multiple definitions, ifs, returns can exist
  while (Next() != tokens::Token::tok_close_curly) {
    if (foundReturn)
      continue;

    auto grammar = G(); // consume grammar

    // LLVM don't accept terminators in the middle of a block
    // so we must continue the while loop until we found a }.
    if (llvm::isa<llvm::ReturnInst>(grammar)) {
      foundReturn = true;
      continue;
    }
  }

  return BB;
}
} // namespace analysis
