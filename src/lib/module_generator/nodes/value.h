#pragma once
#include "llvm/IR/Value.h"

class ExprAST {
public:
  virtual ~ExprAST() = default;

  virtual llvm::Value *codegen() = 0;
};
