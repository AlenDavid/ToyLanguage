#include "module_generator.h"
#include "lib/module_generator/nodes/function.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace generators {
ModuleGenerator::ModuleGenerator()
    : Module(std::make_unique<llvm::Module>("toy language", Context)) {}

void ModuleGenerator::Generate() {
  auto fn = nodes::FunctionAST("main", std::vector<std::string>());
  fn.codegen(Module.get());
}
} // namespace generators
