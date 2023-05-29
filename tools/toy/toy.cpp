#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"

#include <string>
#include <iostream>
#include <fstream>

#include "../../src/lib/input_parser/input_parser.hh"

enum Token
{
  tok_eof = -1,

  // commands
  tok_def = -2,

  // primary
  tok_identifier = -14,
  tok_number = -15,

  // control
  tok_return = -50,
};

class SyntaxChecker
{
private:
  int LastChar = ' ';
  int CaretPlace = 0;
  int CurrentLine = 0;
  int CharCounter = 0;

  std::string Identifier;
  double NumericValue;
  std::string Code;

public:
  std::vector<Token> Tokens;

  explicit SyntaxChecker(std::string Code) : Code(std::move(Code)) {}

  int NextChar()
  {
    auto c = Code.at(CharCounter);
    CharCounter++;
    CaretPlace++;

    if (c == '\n')
    {
      CaretPlace = 0;
      CurrentLine++;
    }

    return c;
  }

  Token GetToken()
  {
    // Skip any whitespace.
    while (isspace(LastChar))
      LastChar = NextChar();

    if (isalpha(LastChar)) // [a-zA-Z][a-zA-Z0-9]*
    {
      Identifier = (char)LastChar;
      LastChar = NextChar();

      while (isalnum(LastChar))
      {
        Identifier += (char)LastChar;
        LastChar = NextChar();
      }

      if (Identifier == "def")
      {
        Tokens.push_back(tok_def);
        return tok_def;
      }

      if (Identifier == "return")
      {
        Tokens.push_back(tok_return);
        return tok_return;
      }

      Tokens.push_back(tok_identifier);
      return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.')
    { // Number: [0-9.]+
      std::string NumStr;
      do
      {
        NumStr += (char)LastChar;
        LastChar = NextChar();
      } while (isdigit(LastChar));

      NumericValue = strtod(NumStr.c_str(), nullptr);

      Tokens.push_back(tok_number);
      return tok_number;
    }

    if (LastChar == '#')
    {
      // Comment until end of line.
      do
        LastChar = NextChar();
      while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

      if (LastChar != EOF)
      {
        Token t = GetToken();
        Tokens.push_back(t);
        return t;
      }
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
    {
      Tokens.push_back(tok_eof);
      return tok_eof;
    }

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = NextChar();

    Tokens.push_back((Token)ThisChar);
    return (Token)ThisChar;
  }
};

int main(int argc, char **argv)
{
  std::cout << "Toy Language compiler" << std::endl;

  auto code = parse_args(argc, argv);

  if (code == "")
  {
    return 1;
  }

  auto parser = SyntaxChecker(code);

  while (parser.GetToken() < 0)
    ;

  for (auto i : parser.Tokens)
  {

    std::cout << i << std::endl;
  }
}
