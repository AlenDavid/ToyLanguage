#include <string>
#include "../syntax_tokens/syntax_tokens.h"

#ifndef TOY_LIB_SYNTAX_CHECKER_H
#define TOY_LIB_SYNTAX_CHECKER_H

class SyntaxChecker
{
public:
  int LastChar = ' ';
  int CaretPlace = 0;
  int CurrentLine = 0;
  int CharCounter = 0;

  std::string CurrentIdentifier;
  double CurrentNumericValue;

  const std::string Code;

  std::vector<Token> Tokens;
  std::vector<std::string> Identifiers;

  explicit SyntaxChecker(std::string Code);

  int NextChar();
  Token GetToken();
};

#endif // TOY_LIB_SYNTAX_CHECKER_H
