#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <string>
#include <iosfwd>
#include "graph.hpp"

namespace velizade
{
  using func_t = std::function<void(std::istream&, std::ostream&)>;

  void cmdGraphs(std::istream& in, std::ostream& out);
  void cmdVertexes(std::istream& in, std::ostream& out);
  void cmdOutbound(std::istream& in, std::ostream& out);
  void cmdInbound(std::istream& in, std::ostream& out);
  void cmdBind(std::istream& in, std::ostream& out);
  void cmdCut(std::istream& in, std::ostream& out);
  void cmdCreate(std::istream& in, std::ostream& out);
  void cmdMerge(std::istream& in, std::ostream& out);
  void cmdExtract(std::istream& in, std::ostream& out);

  class CommandManager
  {
  public:
    CommandManager();

    bool cmd(const std::string& name, std::istream& in, std::ostream& out) const;

    void addCommand(const std::string& name, func_t cmd);

  private:
    using CommandTable = HashTable<std::string, func_t>;
    CommandTable cmds_;
  };
}

#endif
