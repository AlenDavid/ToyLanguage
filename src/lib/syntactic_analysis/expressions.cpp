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
    Next();
    Debug("Token: " + From(CurrentToken));

    if (CurrentToken == Token::tok_open_parenthesis)
    {
      E();
      Next();
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken != Token::tok_close_parenthesis)
      {
        Report(")");
        return false;
      }
    }

    if (CurrentToken == Token::tok_double)
    {
      Next();
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
      Next();
      Debug("Token: " + From(CurrentToken) + " \"" + Factory.CurrentIdentifier + "\"");

      // consume "
      Next();
      Debug("Token: " + From(CurrentToken));

      if (CurrentToken != Token::tok_string)
      {
        Report("\"");
        return false;
      }

      Next();
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
