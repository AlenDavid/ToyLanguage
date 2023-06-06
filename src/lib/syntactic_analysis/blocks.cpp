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
    // For debugging purposes.
    NestLevel++;
    Debug("B()");

    // <DEFS>
    auto g = G();

    Next();

    if (CurrentToken != Token::tok_close_curly)
    {
      Report(";");
      return false;
    }

    NestLevel--;

    return g;
  }
} // namespace analysis
