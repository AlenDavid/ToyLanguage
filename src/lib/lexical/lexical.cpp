#include <string>
#include <iostream>

#include "./lexical.h"
#include "lib/tokens/tokens.h"

using namespace tokens;

namespace lexical
{
  LexicalFactory::LexicalFactory(std::string Code) : Code(std::move(Code)) {}
  void LexicalFactory::EnableDebug()
  {
    Debug = true;
  }

  int LexicalFactory::NextChar()
  {
    if (Code.length() <= CharCounter)
      return EOF;

    char c = Code[CharCounter];
    CharCounter++;
    CaretPlace++;

    if (c == '\n' || c == '\r')
    {
      CaretPlace = 0;
      CurrentLine++;
    }

    return c;
  }

  Token LexicalFactory::NextToken()
  {
    // Skip any whitespace.
    while (isspace(CurrentChar))
      CurrentChar = NextChar();

    if (ispunct(CurrentChar) && CurrentChar != '#') // [!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~]
    {
      CurrentIdentifier = (char)CurrentChar;
      CurrentChar = NextChar();

      if (CurrentIdentifier == "\"")
      {
        Tokens.push_back(Token::tok_string);
        return Token::tok_string;
      }

      if (CurrentIdentifier == "=")
      {
        Tokens.push_back(Token::tok_equal);
        return Token::tok_equal;
      }

      if (CurrentIdentifier == ">")
      {
        Tokens.push_back(Token::tok_greater_than);
        return Token::tok_greater_than;
      }

      if (CurrentIdentifier == "+")
      {
        Tokens.push_back(Token::tok_plus);
        return Token::tok_plus;
      }

      if (CurrentIdentifier == "-")
      {
        Tokens.push_back(Token::tok_minus);
        return Token::tok_minus;
      }

      if (CurrentIdentifier == "*")
      {
        Tokens.push_back(Token::tok_times);
        return Token::tok_times;
      }

      if (CurrentIdentifier == "/")
      {
        Tokens.push_back(Token::tok_divider);
        return Token::tok_divider;
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

    if (isalpha(CurrentChar)) // [a-zA-Z][a-zA-Z0-9]*
    {
      CurrentIdentifier = (char)CurrentChar;
      CurrentChar = NextChar();

      while (isalnum(CurrentChar))
      {
        CurrentIdentifier += (char)CurrentChar;
        CurrentChar = NextChar();
      }

      if (CurrentIdentifier == "def")
      {
        Tokens.push_back(Token::tok_def);
        return Token::tok_def;
      }

      if (CurrentIdentifier == "if")
      {
        Tokens.push_back(Token::tok_if);
        return Token::tok_if;
      }

      if (CurrentIdentifier == "for")
      {
        Tokens.push_back(Token::tok_for);
        return Token::tok_for;
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

    if (isdigit(CurrentChar))
    { // Number: [0-9.]+
      std::string NumStr;
      bool AlreadyUsedDot = false;
      do
      {
        if (!AlreadyUsedDot && (char)CurrentChar == '.')
        {
          AlreadyUsedDot = true;
        }

        NumStr += (char)CurrentChar;
        CurrentChar = NextChar();
      } while (isdigit(CurrentChar) || (!AlreadyUsedDot && CurrentChar == '.'));

      CurrentNumericValue = strtod(NumStr.c_str(), nullptr);

      Identifiers.push_back(NumStr);
      Tokens.push_back(Token::tok_double);
      return Token::tok_double;
    }

    if (CurrentChar == '#')
    {
      // Comment until end of line.
      do
        CurrentChar = NextChar();
      while (CurrentChar != EOF && CurrentChar != '\n' && CurrentChar != '\r');

      if (CurrentChar != EOF)
      {
        Token t = NextToken();
        Tokens.push_back(t);
        return t;
      }
    }

    // Check for end of file.  Don't eat the EOF.
    if (CurrentChar == EOF)
    {
      Tokens.push_back(Token::tok_eof);
      return Token::tok_eof;
    }

    // Otherwise, just return the character as its ascii value.
    int ThisChar = CurrentChar;
    CurrentChar = NextChar();

    Tokens.push_back((Token)ThisChar);
    return (Token)ThisChar;
  }
}
