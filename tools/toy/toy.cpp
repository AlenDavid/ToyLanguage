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
  tok_equal = -3,

  // primary
  tok_identifier = -21,
  tok_int = -22,
  tok_double = -23,
  tok_string = -24,

  // control
  tok_end = -50,
  tok_return = -51,
  tok_open_parenthesis = -52,
  tok_close_parenthesis = -53,
  tok_open_curly = -54,
  tok_close_curly = -55,
};

class SyntaxChecker
{
public:
  int LastChar = ' ';
  int CaretPlace = 0;
  int CurrentLine = 0;
  int CharCounter = 0;

  std::string CurrentIdentifier;
  double CurrentNumericValue;

  const std::string Code;

  std::vector<Token> Tokens;
  std::vector<std::string> Identifiers;

  explicit SyntaxChecker(std::string Code) : Code(std::move(Code)) {}

  int NextChar()
  {
    if (Code.length() <= CharCounter)
      return EOF;

    char c = Code[CharCounter];
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

    if (ispunct(LastChar)) // [!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~]
    {
      CurrentIdentifier = (char)LastChar;
      LastChar = NextChar();

      if (CurrentIdentifier == "=")
      {
        Tokens.push_back(tok_equal);
        return tok_equal;
      }

      if (CurrentIdentifier == "{")
      {
        Tokens.push_back(tok_open_curly);
        return tok_open_curly;
      }

      if (CurrentIdentifier == "}")
      {
        Tokens.push_back(tok_close_curly);
        return tok_close_curly;
      }

      if (CurrentIdentifier == "(")
      {
        Tokens.push_back(tok_open_parenthesis);
        return tok_open_parenthesis;
      }

      if (CurrentIdentifier == ")")
      {
        Tokens.push_back(tok_close_parenthesis);
        return tok_close_parenthesis;
      }

      if (CurrentIdentifier == ";")
      {
        Tokens.push_back(tok_end);
        return tok_end;
      }
    }

    if (isalpha(LastChar)) // [a-zA-Z][a-zA-Z0-9]*
    {
      CurrentIdentifier = (char)LastChar;
      LastChar = NextChar();

      while (isalnum(LastChar))
      {
        CurrentIdentifier += (char)LastChar;
        LastChar = NextChar();
      }

      if (CurrentIdentifier == "def")
      {
        Tokens.push_back(tok_def);
        return tok_def;
      }

      if (CurrentIdentifier == "return")
      {
        Tokens.push_back(tok_return);
        return tok_return;
      }

      Tokens.push_back(tok_identifier);
      Identifiers.push_back(CurrentIdentifier);

      return tok_identifier;
    }

    if (isdigit(LastChar))
    { // Number: [0-9.]+
      std::string NumStr;
      bool AlreadyUsedDot = false;
      do
      {
        if (!AlreadyUsedDot && (char)LastChar == '.')
        {
          AlreadyUsedDot = true;
        }

        NumStr += (char)LastChar;
        LastChar = NextChar();
      } while (isdigit(LastChar) || (!AlreadyUsedDot));

      CurrentNumericValue = strtod(NumStr.c_str(), nullptr);

      Identifiers.push_back(NumStr);
      Tokens.push_back(tok_double);
      return tok_double;
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

  while (parser.GetToken() != tok_eof)
    ;

  std::cout << "Tokens:\n\n";
  for (auto i : parser.Tokens)
  {
    std::cout << i << std::endl;
  }

  std::cout << "Identifiers:\n\n";
  for (auto i : parser.Identifiers)
  {
    std::cout << i << std::endl;
  }
}
