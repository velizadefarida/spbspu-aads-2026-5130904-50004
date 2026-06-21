#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <istream>
#include <ostream>
#include <string>
#include "bstree.hpp"

namespace velizade
{
  class Cmd
  {
  public:
    using DictTree = BSTree<int, std::string>;
    using DictContainer = BSTree<std::string, DictTree>;

    Cmd();

    void loadFromFile(const std::string& filename);
    void processCommands(std::istream& in, std::ostream& out);

  private:
    DictContainer datasets_;

    void processPrint(std::istream& in, std::ostream& out);
    void processComplement(std::istream& in, std::ostream& out);
    void processIntersect(std::istream& in, std::ostream& out);
    void processUnion(std::istream& in, std::ostream& out);
  };
}

#endif
