#include "commands.hpp"
#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <sstream>

velizade::CommandDispatcher::CommandDispatcher()
{
  commands_.insert("add", &CommandDispatcher::cmdAdd);
  commands_.insert("show", &CommandDispatcher::cmdShow);
  commands_.insert("add_book", &CommandDispatcher::cmdAddBook);
  commands_.insert("remove", &CommandDispatcher::cmdRemove);
  commands_.insert("status", &CommandDispatcher::cmdStatus);
  commands_.insert("list", &CommandDispatcher::cmdList);
  commands_.insert("filter", &CommandDispatcher::cmdFilter);
  commands_.insert("filter_not", &CommandDispatcher::cmdFilterNot);
  commands_.insert("merge", &CommandDispatcher::cmdMerge);
  commands_.insert("union", &CommandDispatcher::cmdUnion);
  commands_.insert("complement", &CommandDispatcher::cmdComplement);
  commands_.insert("export", &CommandDispatcher::cmdExport);
  commands_.insert("import", &CommandDispatcher::cmdImport);
}

void velizade::CommandDispatcher::execute(std::istream& in, std::ostream& out)
{
  std::string line;
  while (std::getline(in, line))
  {
    if (line.empty())
    {
      continue;
    }
    size_t pos = 0;
    while (pos < line.size() && std::isspace(line[pos]))
    {
      ++pos;
    }
    if (pos == line.size())
    {
      continue;
    }
    size_t start = pos;
    while (pos < line.size() && !std::isspace(line[pos]))
    {
      ++pos;
    }
    std::string cmd = line.substr(start, pos - start);
    std::string rest = (pos < line.size()) ? line.substr(pos) : "";
    std::stringstream argsStream(rest);

    FuncType func = nullptr;
    if (commands_.find(cmd, func))
    {
      (this->*func)(argsStream, out);
    }
    else
    {
      out << "<INVALID COMMAND> Unknown command.\n";
    }
  }
}

void velizade::CommandDispatcher::require(bool condition) const
{
  if (!condition)
  {
    throw std::runtime_error("Invalid argument");
  }
}

void velizade::CommandDispatcher::requireInput(std::istream& in) const
{
  if (in.fail())
  {
    throw std::runtime_error("Invalid input");
  }
}

bool velizade::CommandDispatcher::isNumberValid(const std::string& str) const
{
  if (str.empty())
  {
    return false;
  }
  for (char c : str)
  {
    if (!std::isdigit(c))
    {
      return false;
    }
  }
  return true;
}

void velizade::CommandDispatcher::cmdAdd(std::istream& in, std::ostream& out)
{
  std::string idStr, author, title, yearStr;
  in >> idStr >> author >> title >> yearStr;
  if (!in)
  {
    out << "<INVALID COMMAND> Wrong number of arguments for add\n";
    return;
  }
  if (!isNumberValid(idStr) || !isNumberValid(yearStr))
  {
    out << "<INVALID COMMAND> ID and year must be numbers\n";
    return;
  }
  int id = std::stoi(idStr);
  int year = std::stoi(yearStr);
  if (core_.addBook(id, author, title, year))
  {
    out << "Book added.\n";
  }
  else
  {
    out << "<INVALID COMMAND> Book with ID " << id << " already exists.\n";
  }
}

void velizade::CommandDispatcher::cmdShow(std::istream& in, std::ostream& out)
{
  std::string idStr;
  in >> idStr;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: show <book_id>\n";
    return;
  }
  if (!isNumberValid(idStr))
  {
    out << "<INVALID COMMAND> ID must be a number\n";
    return;
  }
  int id = std::stoi(idStr);
  Book book;
  if (core_.showBook(id, book))
  {
    out << id << " " << book.author << " " << book.title << " " << book.year << "\n";
  }
  else
  {
    out << "<INVALID COMMAND> Book not found.\n";
  }
}

void velizade::CommandDispatcher::cmdAddBook(std::istream& in, std::ostream& out)
{
  std::string listName, idStr;
  in >> listName >> idStr;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: add_book <user_id> <book_id>\n";
    return;
  }
  if (!isNumberValid(idStr))
  {
    out << "<INVALID COMMAND> Book ID must be a number\n";
    return;
  }
  int bookId = std::stoi(idStr);
  if (core_.addBookToList(listName, bookId))
  {
    out << "Book " << bookId << " added to list " << listName << " (status: unread).\n";
  }
  else
  {
    out << "<INVALID COMMAND> Failed to add book (book does not exist or already in list).\n";
  }
}

void velizade::CommandDispatcher::cmdRemove(std::istream& in, std::ostream& out)
{
  std::string listName, idStr;
  in >> listName >> idStr;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: remove <user_id> <book_id>\n";
    return;
  }
  if (!isNumberValid(idStr))
  {
    out << "<INVALID COMMAND> Book ID must be a number\n";
    return;
  }
  int bookId = std::stoi(idStr);
  if (core_.removeFromList(listName, bookId))
  {
    out << "Book " << bookId << " removed from list " << listName << ".\n";
  }
  else
  {
    out << "<INVALID COMMAND> Book " << bookId << " not in list " << listName << ".\n";
  }
}

void velizade::CommandDispatcher::cmdStatus(std::istream& in, std::ostream& out)
{
  std::string listName, idStr, status;
  in >> listName >> idStr >> status;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: status <user_id> <book_id> <unread|reading|finished>\n";
    return;
  }
  if (!isNumberValid(idStr))
  {
    out << "<INVALID COMMAND> Book ID must be a number\n";
    return;
  }
  if (status != "unread" && status != "reading" && status != "finished")
  {
    out << "<INVALID COMMAND> Invalid status.\n";
    return;
  }
  int bookId = std::stoi(idStr);
  if (core_.setStatus(listName, bookId, status))
  {
    out << "Status of book " << bookId << " in list " << listName << " changed to " << status << ".\n";
  }
  else
  {
    out << "<INVALID COMMAND> Cannot set status (list is not personal or book not found).\n";
  }
}

void velizade::CommandDispatcher::cmdList(std::istream& in, std::ostream& out)
{
  std::string listName;
  in >> listName;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: list <user_id>\n";
    return;
  }
  Vector<std::string> output;
  if (core_.listList(listName, output))
  {
    for (size_t i = 0; i < output.getSize(); ++i)
    {
      out << output[i] << "\n";
    }
  }
  else
  {
    out << "<INVALID COMMAND> List " << listName << " does not exist.\n";
  }
}

void velizade::CommandDispatcher::cmdFilter(std::istream& in, std::ostream& out)
{
  std::string newList, source, status;
  in >> newList >> source >> status;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: filter <new_list> <source_list> <status>\n";
    return;
  }
  if (status != "unread" && status != "reading" && status != "finished")
  {
    out << "<INVALID COMMAND> Invalid status.\n";
    return;
  }
  if (core_.filterByStatus(newList, source, status))
  {
    out << "List " << newList << " created as filtered by status " << status << ".\n";
  }
  else
  {
    out << "<INVALID COMMAND> Filtering error (source list is not personal or new list already exists).\n";
  }
}

void velizade::CommandDispatcher::cmdFilterNot(std::istream& in, std::ostream& out)
{
  std::string newList, source, status;
  in >> newList >> source >> status;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: filter_not <new_list> <source_list> <status>\n";
    return;
  }
  if (status != "unread" && status != "reading" && status != "finished")
  {
    out << "<INVALID COMMAND> Invalid status.\n";
    return;
  }
  if (core_.filterNotStatus(newList, source, status))
  {
    out << "List " << newList << " created as books not having status " << status << ".\n";
  }
  else
  {
    out << "<INVALID COMMAND> Filtering error (source list is not personal or new list already exists).\n";
  }
}

void velizade::CommandDispatcher::cmdMerge(std::istream& in, std::ostream& out)
{
  std::string newList, l1, l2;
  in >> newList >> l1 >> l2;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: merge <new_list> <list1> <list2>\n";
    return;
  }
  if (core_.mergeLists(newList, l1, l2))
  {
    out << "List " << newList << " created as union of " << l1 << " and " << l2 << ".\n";
  }
  else
  {
    out << "<INVALID COMMAND> Merge error.\n";
  }
}

void velizade::CommandDispatcher::cmdUnion(std::istream& in, std::ostream& out)
{
  std::string newList, l1, l2;
  in >> newList >> l1 >> l2;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: union <new_list> <list1> <list2>\n";
    return;
  }
  if (core_.intersectLists(newList, l1, l2))
  {
    out << "List " << newList << " created as intersection of " << l1 << " and " << l2 << ".\n";
  }
  else
  {
    out << "<INVALID COMMAND> Intersection error.\n";
  }
}

void velizade::CommandDispatcher::cmdComplement(std::istream& in, std::ostream& out)
{
  std::string newList, l1, l2;
  in >> newList >> l1 >> l2;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: complement <new_list> <list1> <list2>\n";
    return;
  }
  if (core_.complementLists(newList, l1, l2))
  {
    out << "List " << newList << " created as difference of " << l1 << " and " << l2 << ".\n";
  }
  else
  {
    out << "<INVALID COMMAND> Complement error.\n";
  }
}

void velizade::CommandDispatcher::cmdExport(std::istream& in, std::ostream& out)
{
  std::string filename;
  in >> filename;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: export <filename>\n";
    return;
  }
  if (core_.exportData(filename))
  {
    out << "Exported.\n";
  }
  else
  {
    out << "<INVALID COMMAND> Failed to save file.\n";
  }
}

void velizade::CommandDispatcher::cmdImport(std::istream& in, std::ostream& out)
{
  std::string filename;
  in >> filename;
  if (!in)
  {
    out << "<INVALID COMMAND> Usage: import <filename>\n";
    return;
  }
  if (core_.importData(filename))
  {
    out << "Imported.\n";
  }
  else
  {
    out << "<INVALID COMMAND> File not found.\n";
  }
}
