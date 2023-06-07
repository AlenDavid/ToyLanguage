#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

#pragma once
namespace nodes {
class FunctionAST {
  std::string Name;
  std::vector<std::string> Args;
  llvm::Value *Block;
  llvm::Value *ReturnValue;

public:
  FunctionAST(const std::string &, std::vector<std::string>, llvm::Value *,
              llvm::Value *);

  llvm::Function *codegen(llvm::Module *);
  const std::string &getName() const;
};
} // namespace nodes
