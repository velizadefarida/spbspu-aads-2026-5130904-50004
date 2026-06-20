#include <iostream>
#include <fstream>
#include <string>
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
    std::cerr << e.what() << '\n';
    return 1;
  }

  velizade::CommandManager cmdManager;
  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    velizade::Vector<std::string> tokens = velizade::splitLine(line);
    if (tokens.isEmpty())
    {
      continue;
    }
    std::string cmdName = tokens[0];
    velizade::Vector<std::string> args;
    for (size_t i = 1; i < tokens.getSize(); ++i)
    {
      args.pushBack(tokens[i]);
    }
    if (!cmdManager.cmd(cmdName, args, std::cout))
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
