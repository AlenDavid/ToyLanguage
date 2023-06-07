#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include "syntactic_analysis.h"

using namespace tokens;

namespace analysis
{
  // Syntax check for definitions.
  bool SyntaxChecker::D()
  {
    NestLevel++;
    Debug("D()");

    // consume ID.
    Next();
    Debug("Token: " + From(CurrentToken) + " " + Factory.CurrentIdentifier);

    if (CurrentToken != Token::tok_identifier)
    {
      Report("name");
      NestLevel++;

      return false;
    }

    Next();

    // <DEF> <ID> = <E>
    if (CurrentToken == Token::tok_equal)
    {
      NestLevel--;
      return E();
    }

    // <DEF> <ID> <PARAMS> <BLOCK>
    // <PARAMS> |== (<PARAM>)
    if (CurrentToken == Token::tok_open_parenthesis)
    {
      // TODO: check parameters
      Next();

      if (CurrentToken != Token::tok_close_parenthesis)
      {
        Report(")");
        NestLevel--;
        return false;
      }

      // consume {
      Next();

      if (CurrentToken != Token::tok_open_curly)
      {
        Report("{");
        NestLevel--;
        return false;
      }

      NestLevel--;
      return B();
    }

    Report("Unkown");
    NestLevel--;
    return false;
  }
} // namespace analysis
