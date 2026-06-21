#include "commands.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: filename required\n";
    return 1;
  }

  velizade::Cmd manager;
  try
  {
    manager.loadFromFile(argv[1]);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  manager.processCommands(std::cin, std::cout);
  return 0;
}
