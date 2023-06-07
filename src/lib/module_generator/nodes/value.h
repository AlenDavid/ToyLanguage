#pragma once
#include <llvm/IR/Value.h>

namespace nodes {
class ExprAST {
public:
  virtual ~ExprAST() = default;

  virtual llvm::Value *codegen() = 0;
};
} // namespace nodes
