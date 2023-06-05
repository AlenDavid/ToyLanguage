#ifndef TOY_LIB_SYNTACTIC_ANALYSIS_H
#define TOY_LIB_SYNTACTIC_ANALYSIS_H

namespace analysis
{
  class SyntaxChecker
  {
  public:
    bool Debug = false;

    explicit SyntaxChecker();
    void EnableDebug();
  };
} // namespace analysis

#endif // TOY_LIB_SYNTACTIC_ANALYSIS_H
