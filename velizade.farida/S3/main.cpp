#include <iostream>
#include <exception>
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
    velizade::Application app;
    app.run(argv[1]);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 2;
  }
  catch (...)
  {
    std::cerr << "Unknown error\n";
    return 2;
  }

  return 0;
}
