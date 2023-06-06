#pragma once

class Messages
{
public:
  static constexpr const auto USAGE = R"(Toy Language compiler

Usage:

  toy '<path_to_file.toy>' '<path_to_output>' [options]

Examples:
  toy --help
  toy ./main.toy main
  toy ./main.toy main --verbose

Options:
  --help: print this message
  --verbose: enable syntax logging
  )";
};
