#include "lib/tokens/tokens.h"
#include <string>
#include <vector>

#pragma once
namespace lexical {
class LexicalFactory {
private:
  tokens::Token punctTokens();
  tokens::Token alphaTokens();

public:
  bool Debug = false;
  int CurrentChar = ' ';
  int CaretPlace = 0;
  int CurrentLine = 0;
  int CharCounter = 0;

  std::string CurrentIdentifier;
  double CurrentFloatValue;
  int CurrentIntValue;

  const std::string Code;

  std::vector<tokens::Token> Tokens;
  std::vector<std::string> Identifiers;

  explicit LexicalFactory(std::string Code);

  void EnableDebug();
  int NextChar();
  tokens::Token NextToken();
};
} // namespace lexical
