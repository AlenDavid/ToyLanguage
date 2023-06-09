#include "variable.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <iostream>
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

using namespace llvm;

namespace nodes {
VariableAST::VariableAST(const std::string &Name, llvm::Value *Value)
    : Name(Name), Value(std::move(Value)) {}

const std::string &VariableAST::getName() const { return Name; }

llvm::Value *VariableAST::codegen(llvm::IRBuilder<> *Builder) {
  if (!Value || !Builder)
    return nullptr;

  auto *A = Builder->CreateAlloca(Value->getType(), nullptr, Name);

  if (!A) {
    errs() << "could't allocate " << Name << '\n';
    return nullptr;
  }

  auto store = Builder->CreateStore(Value, A, false);

  if (!store) {
    errs() << "could't store " << Name << '\n';
    return nullptr;
  }

  return Value;
}
} // namespace nodes
