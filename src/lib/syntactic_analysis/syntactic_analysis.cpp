#include <string>

#include "./syntactic_analysis.h"

namespace analysis
{
  SyntaxChecker::SyntaxChecker(lexical::LexicalFactory &factory) : Factory(factory) {}

  void SyntaxChecker::EnableDebug()
  {
    Debug = true;
  }

  void SyntaxChecker::G()
  {
    while (Factory.GetToken() != tokens::Token::tok_eof)
      ;
  }
} // namespace analysis
