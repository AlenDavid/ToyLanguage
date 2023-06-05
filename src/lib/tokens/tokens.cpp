#include <string>

#include "./tokens.h"

namespace tokens
{
  std::string From(Token token)
  {
    if (token == Token::tok_eof)
      return "EOF";
    if (token == Token::tok_def)
      return "def";
    if (token == Token::tok_equal)
      return "equal";
    if (token == Token::tok_plus)
      return "plus";
    if (token == Token::tok_minus)
      return "minus";
    if (token == Token::tok_times)
      return "times";
    if (token == Token::tok_divider)
      return "divider";
    if (token == Token::tok_identifier)
      return "identifier";
    if (token == Token::tok_int)
      return "int";
    if (token == Token::tok_double)
      return "double";
    if (token == Token::tok_string)
      return "string";
    if (token == Token::tok_end)
      return "end";
    if (token == Token::tok_return)
      return "return";
    if (token == Token::tok_open_parenthesis)
      return "open_parenthesis";
    if (token == Token::tok_close_parenthesis)
      return "close_parenthesis";
    if (token == Token::tok_open_curly)
      return "open_curly";
    if (token == Token::tok_close_curly)
      return "close_curly";
    if (token == Token::tok_if)
      return "if";
    if (token == Token::tok_for)
      return "for";
    if (token == Token::tok_greater_than)
      return "greater_than";

    return "unkown";
  }
}
