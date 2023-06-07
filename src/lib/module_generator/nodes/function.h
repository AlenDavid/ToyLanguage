#include <llvm/IR/Function.h>
#include <string>
#include <vector>

#pragma once
namespace nodes {
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &Name, std::vector<std::string> Args);

  llvm::Function *codegen();
  const std::string &getName();
};
} // namespace nodes
