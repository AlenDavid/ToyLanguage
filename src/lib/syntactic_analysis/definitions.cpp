#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include "./syntactic_analysis.h"

using namespace tokens;

namespace analysis
{
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
} // namespace analysis
