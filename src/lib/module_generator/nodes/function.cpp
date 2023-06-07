#include "function.h"
#include "llvm/IR/LLVMContext.h"
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

namespace nodes {
PrototypeAST::PrototypeAST(const std::string &Name,
                           std::vector<std::string> Args)
    : Name(Name), Args(std::move(Args)) {}

const std::string &PrototypeAST::getName() { return Name; }

llvm::Function *PrototypeAST::codegen(llvm::Module *Module) {
  auto Context = llvm::LLVMContext();

  std::vector<llvm::Type *> Empty(0);

  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), Empty, false);

  llvm::Function *TheFunction =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, Module);

  return TheFunction;
}
} // namespace nodes
