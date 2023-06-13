#include <string>

#ifndef TOY_LIB_TOKENS_H
#define TOY_LIB_TOKENS_H

namespace tokens {
enum class Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_equal = -3,
  tok_plus = -4,
  tok_minus = -5,
  tok_times = -6,
  tok_divider = -7,

  // primary
  tok_identifier = -21,
  tok_int = -22,
  tok_double = -23,
  tok_string = -24,

  // control
  tok_end = -50,
  tok_return = -51,
  tok_open_parenthesis = -52,
  tok_close_parenthesis = -53,
  tok_open_curly = -54,
  tok_close_curly = -55,
  tok_if = -56,
  tok_for = -57,
  tok_greater_than = -58,
  tok_smaller_than = -59,
  tok_double_equals = -60,

  // helpers only
  no_token = -999,
};

std::string From(Token);
} // namespace tokens

#endif // TOY_LIB_TOKENS_H
