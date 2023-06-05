#include <string>
#include "../tokens/tokens.h"

#include "./syntactic_analysis.h"

namespace analysis
{
  SyntaxChecker::SyntaxChecker() = default;

  void SyntaxChecker::EnableDebug()
  {
    Debug = true;
  }
} // namespace analysis
