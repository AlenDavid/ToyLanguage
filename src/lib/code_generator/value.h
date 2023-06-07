#pragma once
#include "llvm/IR/Module.h"
#include <llvm/IR/Value.h>

#include <memory>

namespace nodes {
class ExprAST {
public:
  virtual ~ExprAST() = default;

  virtual llvm::Value *codegen(llvm::Module *) = 0;
};
} // namespace nodes
