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

  void SyntaxChecker::Debug(std::string message)
  {
    if (_Debug)
      std::cout << message << std::endl;
  }

  void SyntaxChecker::Report(std::string expected)
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

    if (CurrentToken == Token::tok_string)
    {
      CurrentToken = Factory.NextToken();
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken != Token::tok_end)
      {
        Report(";");
        return false;
      }
    }

    return false;
  }

  // For grammar checking
  void SyntaxChecker::G()
  {
    Debug("G()");

    for (CurrentToken = Factory.NextToken(); CurrentToken != Token::tok_eof; CurrentToken = Factory.NextToken())
    {
      Debug("Token: " + From(CurrentToken));

      // <BLOCK> |== "{" <DEFS> ";" <RETURN> <VALUE> ";" "}"
      if (CurrentToken == Token::tok_open_curly)
      {
        Debug("BEFORE G");
        G();
        Debug("AFTER G");

        CurrentToken = Factory.NextToken();

        if (CurrentToken != Token::tok_close_curly)
        {
          Report("}");
        }

        break;
      }

      if (CurrentToken == Token::tok_return)
      {
        auto e = E();

        Debug("return " + std::to_string(Factory.CurrentNumericValue));

        if (!e)
        {
          Report("expression");
        }

        break;
      }

      if (CurrentToken == Token::tok_def)
      {
        CurrentToken = Factory.NextToken();
        Debug("Token: " + From(CurrentToken) + " " + Factory.CurrentIdentifier);

        if (CurrentToken != Token::tok_identifier)
        {
          Report("name");
          break;
        }

        CurrentToken = Factory.NextToken();
        Debug("Token: " + From(CurrentToken));

        // <DEF> <ID> = <E>
        if (CurrentToken == Token::tok_equal)
        {
          if (!E())
          {
            Report("def");
            break;
          }
        }

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
            break;
          }
        }

        if (CurrentToken == Token::tok_eof)
        {
          Report("code");
          break;
        }
      }
    }
  }
} // namespace analysis
