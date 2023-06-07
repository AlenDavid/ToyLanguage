#pragma once

#include "llvm/IR/Attributes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace generators {
class ModuleGenerator {
public:
  llvm::LLVMContext Context = llvm::LLVMContext();
  std::unique_ptr<llvm::Module> Module;

  ModuleGenerator()
      : Module(std::make_unique<llvm::Module>("toy language", Context)) {}

  void Generate();
};
} // namespace generators
