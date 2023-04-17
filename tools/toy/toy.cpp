#include <fstream>
#include <string>
#include <iostream>

int main(int argc,  char** argv)
{
  if (argc <= 2) {
    std::cout << "usage: 'toy <path_to_file.toy>' '<path_to_output>'" << std::endl;
    return 0;
  }

  auto filename = argv[1];
  auto output = argv[2];

  std::ifstream ifs(filename);
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );

  std::cout << content << std::endl;

  return 0;
}
