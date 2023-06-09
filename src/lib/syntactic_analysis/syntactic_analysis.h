#include <string>
#include <vector>

#include "lib/lexical/lexical.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Error.h"

#pragma once
namespace analysis {
class SyntaxChecker {
public:
  llvm::LLVMContext Context = llvm::LLVMContext();

  std::unique_ptr<llvm::Module> Module;
  std::unique_ptr<llvm::IRBuilder<>> Builder;

  int NestLevel = 0;
  bool _Debug = false;

  std::vector<std::string> Errs;

  lexical::LexicalFactory &Factory;
  tokens::Token CurrentToken;

  explicit SyntaxChecker(lexical::LexicalFactory &factory);

  tokens::Token Next();
  llvm::BasicBlock *B(llvm::BasicBlock *);
  llvm::Value *D();
  llvm::Value *E();
  llvm::Value *G();

  llvm::Expected<std::unique_ptr<SyntaxChecker>> Codegen();

  void EnableDebug();
  void Debug(const std::string &) const;
  void Report(const std::string &);
};
} // namespace analysis
