#include <iostream>
#include <fstream>
#include <limits>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  try
  {
    velizade::loadGraphsFromFile(argv[1]);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  velizade::CommandManager cmdManager;

  std::string command;
  while (std::cin >> command)
  {
    if (!cmdManager.cmd(command, std::cin, std::cout))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), "\n");
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }

  return 0;
}
