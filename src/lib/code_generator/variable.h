#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

#pragma once
namespace nodes {
class VariableAST {
  std::string Name;
  llvm::Value *Value;

public:
  VariableAST(const std::string &, llvm::Value *);

  llvm::Value *codegen(llvm::IRBuilder<> *);
  const std::string &getName() const;
};
} // namespace nodes
