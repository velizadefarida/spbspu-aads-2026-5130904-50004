#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include "library_manager.hpp"
#include "avl_tree.hpp"

namespace velizade
{
  class CommandDispatcher
  {
  public:
    using FuncType = void (CommandDispatcher::*)(std::istream&, std::ostream&);

    CommandDispatcher();
    void execute(std::istream& in, std::ostream& out);

  private:
    LibraryManager core_;
    AVLTree<std::string, FuncType> commands_;

    void cmdAdd(std::istream& in, std::ostream& out);
    void cmdShow(std::istream& in, std::ostream& out);
    void cmdAddBook(std::istream& in, std::ostream& out);
    void cmdRemove(std::istream& in, std::ostream& out);
    void cmdStatus(std::istream& in, std::ostream& out);
    void cmdList(std::istream& in, std::ostream& out);
    void cmdFilter(std::istream& in, std::ostream& out);
    void cmdFilterNot(std::istream& in, std::ostream& out);
    void cmdMerge(std::istream& in, std::ostream& out);
    void cmdUnion(std::istream& in, std::ostream& out);
    void cmdComplement(std::istream& in, std::ostream& out);
    void cmdExport(std::istream& in, std::ostream& out);
    void cmdImport(std::istream& in, std::ostream& out);

    bool isNumberValid(const std::string& str) const;
  };
}

#endif
