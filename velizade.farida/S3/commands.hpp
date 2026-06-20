#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <string>
#include <iosfwd>
#include "graph.hpp"
#include "vector.hpp"

namespace velizade
{

  using func_t = std::function<void(const Vector<std::string>&, std::ostream&)>;

  void cmdGraphs(const Vector<std::string>& args, std::ostream& out);
  void cmdVertexes(const Vector<std::string>& args, std::ostream& out);
  void cmdOutbound(const Vector<std::string>& args, std::ostream& out);
  void cmdInbound(const Vector<std::string>& args, std::ostream& out);
  void cmdBind(const Vector<std::string>& args, std::ostream& out);
  void cmdCut(const Vector<std::string>& args, std::ostream& out);
  void cmdCreate(const Vector<std::string>& args, std::ostream& out);
  void cmdMerge(const Vector<std::string>& args, std::ostream& out);
  void cmdExtract(const Vector<std::string>& args, std::ostream& out);

  class CommandManager
  {
  public:
    CommandManager();

    bool cmd(const std::string& name, const Vector<std::string>& args, std::ostream& out) const;

    void addCommand(const std::string& name, func_t cmd);

  private:
    using CommandTable = HashTable<std::string, func_t>;
    CommandTable cmds_;
  };

}

#endif
