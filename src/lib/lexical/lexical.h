#include "lib/tokens/tokens.h"
#include <string>
#include <vector>

#pragma once
namespace lexical {
class LexicalFactory {
private:
  tokens::Token _NextToken();

public:
  bool Debug = false;
  int CurrentChar = ' ';
  int CaretPlace = 0;
  int CurrentLine = 0;
  int CharCounter = 0;

  std::string CurrentIdentifier;
  double CurrentNumericValue;

  const std::string Code;

  std::vector<tokens::Token> Tokens;
  std::vector<std::string> Identifiers;

  explicit LexicalFactory(std::string Code);

  void EnableDebug();
  int NextChar();
  tokens::Token NextToken();
};
} // namespace lexical
