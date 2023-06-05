#include "../lexical/lexical.h"

#ifndef TOY_LIB_SYNTACTIC_ANALYSIS_H
#define TOY_LIB_SYNTACTIC_ANALYSIS_H

namespace analysis
{
  class SyntaxChecker
  {
  public:
    bool Debug = false;
    lexical::LexicalFactory &Factory;

    explicit SyntaxChecker(lexical::LexicalFactory &factory);
    void EnableDebug();
    void G();
  };
} // namespace analysis

#endif // TOY_LIB_SYNTACTIC_ANALYSIS_H
