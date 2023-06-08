#include "variable.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include <iostream>
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

namespace nodes {
VariableAST::VariableAST(const std::string &Name, llvm::Value *Value)
    : Name(Name), Value(std::move(Value)) {}

const std::string &VariableAST::getName() const { return Name; }

llvm::Value *VariableAST::codegen([[maybe_unused]] llvm::Module *Module) {
  return Value;
}
} // namespace nodes
