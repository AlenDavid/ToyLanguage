#include <string>
#include <vector>

#include "lib/lexical/lexical.h"

#pragma once
namespace analysis {
class SyntaxChecker {
public:
  int NestLevel = 0;
  bool _Debug = false;

  std::vector<std::string> Errs;
  std::vector<tokens::Token> ExpressionTokens = {tokens::Token::tok_identifier,
                                                 tokens::Token::tok_return};

  lexical::LexicalFactory &Factory;
  tokens::Token CurrentToken;

  explicit SyntaxChecker(lexical::LexicalFactory &factory);

  tokens::Token Next();
  bool B();
  bool D();
  bool E();
  bool G();

  void EnableDebug();
  void Debug(const std::string &) const;
  void Report(const std::string &);
};
} // namespace analysis
