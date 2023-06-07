#include "module_generator.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"
#include <algorithm>
#include <memory>

namespace generators {

void ModuleGenerator::Generate() {
  auto Builder = llvm::IRBuilder(Context);
  llvm::ExitOnError ExitOnErr;

  std::vector<llvm::Type *> Empty(0);
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), Empty, false);

  llvm::Function *TheFunction = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "main", *Module.get());
  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(Context, "entry", TheFunction);

  Builder.SetInsertPoint(BB);

  if (llvm::Value *RetVal =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 27)) {
    // Finish off the function.
    Builder.CreateRet(RetVal);
  }
}
} // namespace generators
