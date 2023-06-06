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
    NestLevel++;
    Debug("E()");
    Next();

    if (CurrentToken == Token::tok_open_parenthesis)
    {
      E();
      Next();

      if (CurrentToken != Token::tok_close_parenthesis)
      {
        Report(")");
        NestLevel--;

        return false;
      }
    }

    if (CurrentToken == Token::tok_double)
    {
      Next();

      if (CurrentToken == Token::tok_end)
      {
        NestLevel--;
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

      if (CurrentToken != Token::tok_string)
      {
        Report("\"");
        NestLevel--;
        return false;
      }

      Next();

      if (CurrentToken != Token::tok_end)
      {
        Report(";");
        NestLevel--;
        NestLevel--;
        return false;
      }

      NestLevel--;
      return true;
    }

    return false;
  }
} // namespace analysis
