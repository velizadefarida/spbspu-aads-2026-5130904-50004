#include <iostream>
#include <fstream>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  velizade::CommandDispatcher dispatcher;
  if (argc == 2)
  {
    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "Cannot open file\n";
      return 1;
    }
    dispatcher.execute(file, std::cout);
  }
  else
  {
    dispatcher.execute(std::cin, std::cout);
  }
  return 0;
}
