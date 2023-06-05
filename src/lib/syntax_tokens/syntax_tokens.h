#ifndef TOY_LIB_SYNTAX_TOKENS_H
#define TOY_LIB_SYNTAX_TOKENS_H
namespace tokens
{
  enum class Token
  {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_equal = -3,

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
  };
}

#endif
