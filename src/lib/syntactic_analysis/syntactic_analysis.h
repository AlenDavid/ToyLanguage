#include "../lexical/lexical.h"

#ifndef TOY_LIB_SYNTACTIC_ANALYSIS_H
#define TOY_LIB_SYNTACTIC_ANALYSIS_H

namespace analysis
{
  class SyntaxChecker
  {
  public:
    bool Debug = false;
    std::vector<std::string> Errs;
    lexical::LexicalFactory &Factory;

    explicit SyntaxChecker(lexical::LexicalFactory &factory);
    void EnableDebug();
    void G();
    void Report(std::string, tokens::Token);
  };
} // namespace analysis

#endif // TOY_LIB_SYNTACTIC_ANALYSIS_H
