#include <llvm/IR/Function.h>
#include <string>
#include <vector>

#pragma once
namespace nodes {
class NumberExprAST {
  double Val;

public:
  NumberExprAST(double);

  llvm::Value *codegen();
};
} // namespace nodes
