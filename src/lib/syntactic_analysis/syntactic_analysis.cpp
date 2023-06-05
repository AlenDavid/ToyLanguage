#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include "./syntactic_analysis.h"

using namespace tokens;

namespace analysis
{
  SyntaxChecker::SyntaxChecker(lexical::LexicalFactory &factory) : Factory(factory) {}

  void SyntaxChecker::EnableDebug() { _Debug = true; }

  void SyntaxChecker::Debug(const std::string &message)
  {
    if (_Debug)
      std::cout << message << std::endl;
  }

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
