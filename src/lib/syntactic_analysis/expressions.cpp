#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include "./syntactic_analysis.h"

using namespace tokens;

namespace analysis
{

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
} // namespace analysis
