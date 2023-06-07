#pragma once

#include "llvm/IR/Attributes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace generators {
class ModuleGenerator {
  llvm::LLVMContext Context = llvm::LLVMContext();

public:
  std::unique_ptr<llvm::Module> Module;

  ModuleGenerator();

  void Generate();
};
} // namespace generators
