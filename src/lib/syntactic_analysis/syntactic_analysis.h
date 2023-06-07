#include <string>
#include <vector>

#include "lib/lexical/lexical.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Error.h"

#pragma once
namespace analysis {
class SyntaxChecker {
  const std::vector<tokens::Token> ExpressionTokens = {
      tokens::Token::tok_identifier, tokens::Token::tok_return};

public:
  llvm::LLVMContext Context = llvm::LLVMContext();

  std::unique_ptr<llvm::Module> Module;

  int NestLevel = 0;
  bool _Debug = false;

  std::vector<std::string> Errs;

  lexical::LexicalFactory &Factory;
  tokens::Token CurrentToken;

  explicit SyntaxChecker(lexical::LexicalFactory &factory);

  tokens::Token Next();
  bool B();
  bool D();
  bool E();
  bool G();

  llvm::Expected<std::unique_ptr<SyntaxChecker>> Codegen();

  void EnableDebug();
  void Debug(const std::string &) const;
  void Report(const std::string &);
};
} // namespace analysis
