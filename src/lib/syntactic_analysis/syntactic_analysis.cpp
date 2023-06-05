#include <iostream>
#include <string>
#include <sstream>

#include "./syntactic_analysis.h"

using namespace tokens;

namespace analysis
{
  SyntaxChecker::SyntaxChecker(lexical::LexicalFactory &factory) : Factory(factory) {}

  void SyntaxChecker::EnableDebug()
  {
    Debug = true;
  }

  void SyntaxChecker::Report(std::string expected, tokens::Token token)
  {
    std::ostringstream oss;

    oss << "expected: " << expected << " got: ";

    if (token == Token::tok_identifier || token == Token::tok_string)
    {
      oss << Factory.CurrentIdentifier;
    }
    else if (token == Token::tok_double || token == Token::tok_int)
    {
      oss << Factory.CurrentNumericValue;
    }
    else
    {
      oss << tokens::From(token);
    }

    oss << " at line " << Factory.CurrentLine << ", column " << Factory.CaretPlace;

    Errs.emplace_back(oss.str());
    oss.clear();
  }

  void SyntaxChecker::G()
  {
    Token token;

    while (Token::tok_eof != (token = Factory.GetToken()))
    {
      if (token != Token::tok_def)
        Report("def", token);

      token = Factory.GetToken();
      if (token == Token::tok_identifier)
      {
        token = Factory.GetToken();
        if (token == Token::tok_equal)
        {
          token = Factory.GetToken();
          if (token == Token::tok_double)
          {
          }
          else if (token == Token::tok_string)
          {
          }
        }
        else if (token == Token::tok_equal)
        {
        }
      }
    }
  }
} // namespace analysis
