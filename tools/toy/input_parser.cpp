#include <string>
#include <iostream>
#include <fstream>

// @deprecated
std::string parse_args(int argc, char **argv)
{
  if (argc <= 2)
  {
    std::cout << "usage: 'toy <path_to_file.toy>' '<path_to_output>' [options]" << std::endl;
    return std::string("");
  }

  auto filename = argv[1];
  auto output = argv[2];

  std::ifstream ifs(filename);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));

  return content;
}
