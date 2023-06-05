#include "../lexical/lexical.h"

#ifndef TOY_LIB_SYNTACTIC_ANALYSIS_H
#define TOY_LIB_SYNTACTIC_ANALYSIS_H

namespace analysis
{
  class SyntaxChecker
  {
  public:
    bool _Debug = false;
    std::vector<std::string> Errs;
    lexical::LexicalFactory &Factory;
    tokens::Token CurrentToken;

    explicit SyntaxChecker(lexical::LexicalFactory &factory);
    void EnableDebug();
    void Debug(std::string);
    bool E();
    void G();
    void Report(std::string);
  };
} // namespace analysis

#endif // TOY_LIB_SYNTACTIC_ANALYSIS_H
