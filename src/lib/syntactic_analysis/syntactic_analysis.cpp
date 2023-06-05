#include <algorithm>
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
    _Debug = true;
  }

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

  // Syntax check for blocks of code.
  bool SyntaxChecker::B()
  {
    Debug("B()");

    // <DEFS>
    auto g = G();

    CurrentToken = Factory.NextToken();
    Debug("Token: " + From(CurrentToken));

    if (CurrentToken != Token::tok_close_curly)
    {
      Report(";");
      return false;
    }

    return g;
  }

  // Syntax check for definitions.
  bool SyntaxChecker::D()
  {
    Debug("D()");

    // consume ID.
    CurrentToken = Factory.NextToken();
    Debug("Token: " + From(CurrentToken) + " " + Factory.CurrentIdentifier);

    if (CurrentToken != Token::tok_identifier)
    {
      Report("name");
      return false;
    }

    // consume next token.
    CurrentToken = Factory.NextToken();
    Debug("Token: " + From(CurrentToken));

    // <DEF> <ID> = <E>
    if (CurrentToken == Token::tok_equal)
      return E();

    // <DEF> <ID> <PARAMS> <BLOCK>
    // <PARAMS> |== (<PARAM>)
    if (CurrentToken == Token::tok_open_parenthesis)
    {
      // TODO: check parameters
      CurrentToken = Factory.NextToken();
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken != Token::tok_close_parenthesis)
      {
        Report(")");
        return false;
      }

      // consume {
      CurrentToken = Factory.NextToken();
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken != Token::tok_open_curly)
      {
        Report("{");
        return false;
      }

      return B();
    }

    Report("Unkown");
    return false;
  }

  // For expressions
  bool SyntaxChecker::E()
  {
    Debug("E()");
    CurrentToken = Factory.NextToken();
    Debug("Token: " + From(CurrentToken));

    if (CurrentToken == Token::tok_open_parenthesis)
    {
      E();
      CurrentToken = Factory.NextToken();
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken != Token::tok_close_parenthesis)
      {
        Report(")");
        return false;
      }
    }

    if (CurrentToken == Token::tok_double)
    {
      CurrentToken = Factory.NextToken();
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken == Token::tok_end)
      {
        return true;
      }

      if (CurrentToken == Token::tok_greater_than)
      {
        return E();
      }

      Report(";");
      return false;
    }

    // "[\w\W]+"
    if (CurrentToken == Token::tok_string)
    {
      // consume text
      CurrentToken = Factory.NextToken();
      Debug("Token: " + From(CurrentToken) + " \"" + Factory.CurrentIdentifier + "\"");

      // consume "
      CurrentToken = Factory.NextToken();
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken != Token::tok_string)
      {
        Report("\"");
        return false;
      }

      CurrentToken = Factory.NextToken();
      Debug("Token: " + From(CurrentToken));
      if (CurrentToken != Token::tok_end)
      {
        Report(";");
        return false;
      }

      return true;
    }

    return false;
  }

  // For grammar checking
  bool SyntaxChecker::G()
  {
    Debug("G()");

    for (CurrentToken = Factory.NextToken(); CurrentToken != Token::tok_eof; CurrentToken = Factory.NextToken())
    {
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken == Token::tok_def && !D())
        return false;

      if (std::find(ExpressionTokens.begin(), ExpressionTokens.end(), CurrentToken) != ExpressionTokens.end())
      {
        auto e = E();

        if (!e)
        {
          Report("expression");
        }

        return e;
      }
    }

    return true;
  }
} // namespace analysis
