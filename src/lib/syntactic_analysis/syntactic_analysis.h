#include <map>
#include <string>
#include <vector>

#include "lib/lexical/lexical.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/raw_ostream.h"

#pragma once
namespace analysis {
class SyntaxChecker {
public:
  llvm::LLVMContext Context = llvm::LLVMContext();

  std::string errs;
  llvm::raw_string_ostream OS;

  std::unique_ptr<llvm::Module> Module;
  std::unique_ptr<llvm::IRBuilder<>> Builder;

  std::map<std::string, llvm::Value *> Variables;

  int NestLevel = 0;
  bool Errored = false;
  bool _Debug = false;

  lexical::LexicalFactory &Factory;
  tokens::Token CurrentToken;

  explicit SyntaxChecker(lexical::LexicalFactory &factory);

  tokens::Token Next();

  llvm::Value *T();
  llvm::Value *E();
  llvm::Value *G(llvm::Function *Parent);
  llvm::Value *B(llvm::Function *, const std::string &Name = "entry");
  llvm::Value *D();

  llvm::Expected<std::unique_ptr<SyntaxChecker>> Codegen();

  void EnableDebug();
  void Debug(const std::string &) const;
  nullptr_t Expect(const std::string &);
  nullptr_t Error(const std::string &);
};
} // namespace analysis
