#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include "./syntactic_analysis.h"

using namespace tokens;

namespace analysis
{
  // For grammar checking
  bool SyntaxChecker::G()
  {
    NestLevel++;
    Debug("G()");

    for (Next(); CurrentToken != Token::tok_eof; Next())
    {
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

    NestLevel--;

    return true;
  }
} // namespace analysis
