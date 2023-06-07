#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis
{
  SyntaxChecker::SyntaxChecker(lexical::LexicalFactory &factory) : Factory(factory) {}

  // Consume next token from Factory and assign to CurrentToken.
  Token SyntaxChecker::Next()
  {
    CurrentToken = Factory.NextToken();
    Debug("[" + From(CurrentToken) + "]");

    return CurrentToken;
  }

  // Will enable logs from the analysis.
  void SyntaxChecker::EnableDebug() { _Debug = true; }

  // Log message if Debug is true.
  void SyntaxChecker::Debug(const std::string &message)
  {
    if (_Debug)
    {
      for (auto i = 0; i < NestLevel; i++)
        std::cout << "  ";
      std::cout << message << std::endl;
    }
  }

  // Store analysis errors.
  void SyntaxChecker::Report(const std::string &expected)
  {
    std::ostringstream oss;

    oss << "expected: " << expected << " got: ";

    if (CurrentToken == Token::tok_identifier || CurrentToken == Token::tok_string)
    {
      oss << Factory.CurrentIdentifier;
    }

    else if (CurrentToken == Token::tok_double || CurrentToken == Token::tok_int)
    {
      oss << Factory.CurrentNumericValue;
    }

    else
    {
      oss << tokens::From(CurrentToken);
    }

    oss << " at line " << Factory.CurrentLine + 1 << ", column " << Factory.CaretPlace + 1;

    Debug(oss.str());
    Errs.emplace_back(oss.str());

    oss.clear();
  }
} // namespace analysis
