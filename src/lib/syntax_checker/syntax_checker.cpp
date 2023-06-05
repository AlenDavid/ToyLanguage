#include <string>

#include "./syntax_checker.hh"
#include "../syntax_tokens/syntax_tokens.h"

SyntaxChecker::SyntaxChecker(std::string Code) : Code(std::move(Code)) {}

int SyntaxChecker::NextChar()
{
  if (Code.length() <= CharCounter)
    return EOF;

  char c = Code[CharCounter];
  CharCounter++;
  CaretPlace++;

  if (c == '\n')
  {
    CaretPlace = 0;
    CurrentLine++;
  }

  return c;
}

Token SyntaxChecker::GetToken()
{
  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = NextChar();

  if (ispunct(LastChar)) // [!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~]
  {
    CurrentIdentifier = (char)LastChar;
    LastChar = NextChar();

    if (CurrentIdentifier == "=")
    {
      Tokens.push_back(Token::tok_equal);
      return Token::tok_equal;
    }

    if (CurrentIdentifier == "{")
    {
      Tokens.push_back(Token::tok_open_curly);
      return Token::tok_open_curly;
    }

    if (CurrentIdentifier == "}")
    {
      Tokens.push_back(Token::tok_close_curly);
      return Token::tok_close_curly;
    }

    if (CurrentIdentifier == "(")
    {
      Tokens.push_back(Token::tok_open_parenthesis);
      return Token::tok_open_parenthesis;
    }

    if (CurrentIdentifier == ")")
    {
      Tokens.push_back(Token::tok_close_parenthesis);
      return Token::tok_close_parenthesis;
    }

    if (CurrentIdentifier == ";")
    {
      Tokens.push_back(Token::tok_end);
      return Token::tok_end;
    }
  }

  if (isalpha(LastChar)) // [a-zA-Z][a-zA-Z0-9]*
  {
    CurrentIdentifier = (char)LastChar;
    LastChar = NextChar();

    while (isalnum(LastChar))
    {
      CurrentIdentifier += (char)LastChar;
      LastChar = NextChar();
    }

    if (CurrentIdentifier == "def")
    {
      Tokens.push_back(Token::tok_def);
      return Token::tok_def;
    }

    if (CurrentIdentifier == "return")
    {
      Tokens.push_back(Token::tok_return);
      return Token::tok_return;
    }

    Tokens.push_back(Token::tok_identifier);
    Identifiers.push_back(CurrentIdentifier);

    return Token::tok_identifier;
  }

  if (isdigit(LastChar))
  { // Number: [0-9.]+
    std::string NumStr;
    bool AlreadyUsedDot = false;
    do
    {
      if (!AlreadyUsedDot && (char)LastChar == '.')
      {
        AlreadyUsedDot = true;
      }

      NumStr += (char)LastChar;
      LastChar = NextChar();
    } while (isdigit(LastChar) || (!AlreadyUsedDot));

    CurrentNumericValue = strtod(NumStr.c_str(), nullptr);

    Identifiers.push_back(NumStr);
    Tokens.push_back(Token::tok_double);
    return Token::tok_double;
  }

  if (LastChar == '#')
  {
    // Comment until end of line.
    do
      LastChar = NextChar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
    {
      Token t = GetToken();
      Tokens.push_back(t);
      return t;
    }
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
  {
    Tokens.push_back(Token::tok_eof);
    return Token::tok_eof;
  }

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = NextChar();

  Tokens.push_back((Token)ThisChar);
  return (Token)ThisChar;
}
