#include "../lexical/lexical.h"

#ifndef TOY_LIB_SYNTACTIC_ANALYSIS_H
#define TOY_LIB_SYNTACTIC_ANALYSIS_H

namespace analysis
{
  class SyntaxChecker
  {
  public:
    int NestLevel = 0;
    bool _Debug = false;
    std::vector<std::string> Errs;
    std::vector<tokens::Token> ExpressionTokens = {tokens::Token::tok_identifier, tokens::Token::tok_return};

    lexical::LexicalFactory &Factory;
    tokens::Token CurrentToken;

    explicit SyntaxChecker(lexical::LexicalFactory &factory);

    tokens::Token Next();
    bool B();
    bool D();
    bool E();
    bool G();

    void EnableDebug();
    void Debug(const std::string &);
    void Report(const std::string &);
  };
} // namespace analysis

#endif // TOY_LIB_SYNTACTIC_ANALYSIS_H
