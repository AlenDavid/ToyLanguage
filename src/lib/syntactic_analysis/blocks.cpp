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
llvm::BasicBlock *SyntaxChecker::B(llvm::BasicBlock *BB) {
  // For debugging purposes.
  NestLevel++;

  Builder->SetInsertPoint(BB);

  while (Next() == tokens::Token::tok_def) {
    if (!G()) {
      Report("G");
      NestLevel--;
      return nullptr;
    }
  }
  llvm::errs() << "GS\n";

  NestLevel--;
  return BB;
}
} // namespace analysis
