#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include "./syntactic_analysis.h"

using namespace tokens;

namespace analysis
{
  // Syntax check for blocks of code.
  bool SyntaxChecker::B()
  {
    Debug("B()");

    // <DEFS>
    auto g = G();

    Next();
    Debug("Token: " + From(CurrentToken));

    if (CurrentToken != Token::tok_close_curly)
    {
      Report(";");
      return false;
    }

    return g;
  }
} // namespace analysis
