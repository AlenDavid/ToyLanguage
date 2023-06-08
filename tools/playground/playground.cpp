#include <iostream>
#include <memory>
#include <string>

std::unique_ptr<int> dup(int *v) {
  auto n = *v * 2;

  return std::make_unique<int>(n);
}

std::unique_ptr<int> consume_and_return(int *pint) {
  std::cout << "pint is: " << std::to_string(*pint) << std::endl;

  return dup(pint);
}

int main() {
  auto c = std::make_unique<int>(5);
  std::cout << "c is: " << std::to_string(*c) << " address: " << c << std::endl;
  auto d = consume_and_return(c.get());

  std::cout << "c is: " << std::to_string(*c) << " address: " << c << std::endl;
  std::cout << "d is: " << std::to_string(*d) << std::endl;
  return 0;
}
