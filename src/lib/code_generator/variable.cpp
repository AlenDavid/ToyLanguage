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

  errs() << "ctx!\n";
  auto &ctx = Builder->getContext();

  errs() << "A\n";
  auto *A = Builder->CreateAlloca(Type::getInt32Ty(ctx), nullptr, "a");

  errs() << "L\n";
  auto *L = ConstantInt::get(Type::getInt32Ty(ctx), 41);
  errs() << "STORES\n";

  return Builder->CreateStore(L, A, false);
  return Value;
}
} // namespace nodes
