#include <string>
#include "../tokens/tokens.h"

#ifndef TOY_LIB_SYNTAX_CHECKER_H
#define TOY_LIB_SYNTAX_CHECKER_H

namespace lexical
{
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

    std::vector<tokens::Token> Tokens;
    std::vector<std::string> Identifiers;

    explicit SyntaxChecker(std::string Code);

    int NextChar();
    tokens::Token GetToken();
  };
}
#endif // TOY_LIB_SYNTAX_CHECKER_H
