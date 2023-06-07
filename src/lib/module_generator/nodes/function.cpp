#include "function.h"
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

namespace nodes {

PrototypeAST::PrototypeAST(const std::string &Name,
                           std::vector<std::string> Args)
    : Name(Name), Args(std::move(Args)) {}

const std::string &PrototypeAST::getName() { return Name; }

} // namespace nodes
