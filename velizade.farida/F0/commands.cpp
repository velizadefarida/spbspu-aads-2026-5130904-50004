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
      reportError(out, "Unknown command");
    }
  }
}

void velizade::CommandDispatcher::reportError(std::ostream& out, const std::string& msg)
{
  out << "<INVALID COMMAND> " << msg << "\n";
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
    reportError(out, "Wrong number of arguments for add");
    return;
  }
  if (!isNumberValid(idStr) || !isNumberValid(yearStr))
  {
    reportError(out, "ID and year must be numbers");
    return;
  }
  int id = std::stoi(idStr);
  int year = std::stoi(yearStr);
  if (core_.addBook(id, author, title, year))
  {
    out << "Book added\n";
  }
  else
  {
    reportError(out, "Book with ID " + idStr + " already exists");
  }
}

void velizade::CommandDispatcher::cmdShow(std::istream& in, std::ostream& out)
{
  std::string idStr;
  in >> idStr;
  if (!in)
  {
    reportError(out, "Usage: show <book_id>");
    return;
  }
  if (!isNumberValid(idStr))
  {
    reportError(out, "ID must be a number");
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
    reportError(out, "Book not found");
  }
}

void velizade::CommandDispatcher::cmdAddBook(std::istream& in, std::ostream& out)
{
  std::string listName, idStr;
  in >> listName >> idStr;
  if (!in)
  {
    reportError(out, "Usage: add_book <user_id> <book_id>");
    return;
  }
  if (!isNumberValid(idStr))
  {
    reportError(out, "Book ID must be a number");
    return;
  }
  int bookId = std::stoi(idStr);
  if (core_.addBookToList(listName, bookId))
  {
    out << "Book " << bookId << " added to list " << listName << " (status: unread)\n";
  }
  else
  {
    reportError(out, "Failed to add book (book does not exist or already in list)");
  }
}

void velizade::CommandDispatcher::cmdRemove(std::istream& in, std::ostream& out)
{
  std::string listName, idStr;
  in >> listName >> idStr;
  if (!in)
  {
    reportError(out, "Usage: remove <user_id> <book_id>");
    return;
  }
  if (!isNumberValid(idStr))
  {
    reportError(out, "Book ID must be a number");
    return;
  }
  int bookId = std::stoi(idStr);
  if (core_.removeFromList(listName, bookId))
  {
    out << "Book " << bookId << " removed from list " << listName << "\n";
  }
  else
  {
    reportError(out, "Book " + idStr + " not in list " + listName);
  }
}

void velizade::CommandDispatcher::cmdStatus(std::istream& in, std::ostream& out)
{
  std::string listName, idStr, status;
  in >> listName >> idStr >> status;
  if (!in)
  {
    reportError(out, "Usage: status <user_id> <book_id> <unread|reading|finished>");
    return;
  }
  if (!isNumberValid(idStr))
  {
    reportError(out, "Book ID must be a number");
    return;
  }
  if (status != "unread" && status != "reading" && status != "finished")
  {
    reportError(out, "Invalid status");
    return;
  }
  int bookId = std::stoi(idStr);
  if (core_.setStatus(listName, bookId, status))
  {
    out << "Status of book " << bookId << " in list " << listName << " changed to " << status << "\n";
  }
  else
  {
    reportError(out, "Cannot set status (list is not personal or book not found)");
  }
}

void velizade::CommandDispatcher::cmdList(std::istream& in, std::ostream& out)
{
  std::string listName;
  in >> listName;
  if (!in)
  {
    reportError(out, "Usage: list <user_id>");
    return;
  }
  if (core_.listList(listName, out))
  {
  }
  else
  {
    reportError(out, "List " + listName + " does not exist");
  }
}

void velizade::CommandDispatcher::cmdFilter(std::istream& in, std::ostream& out)
{
  std::string newList, source, status;
  in >> newList >> source >> status;
  if (!in)
  {
    reportError(out, "Usage: filter <new_list> <source_list> <status>");
    return;
  }
  if (status != "unread" && status != "reading" && status != "finished")
  {
    reportError(out, "Invalid status");
    return;
  }
  if (core_.filterByStatus(newList, source, status))
  {
    out << "List " << newList << " created as filtered by status " << status << "\n";
  }
  else
  {
    reportError(out, "Filtering error (source list is not personal or new list already exists)");
  }
}

void velizade::CommandDispatcher::cmdFilterNot(std::istream& in, std::ostream& out)
{
  std::string newList, source, status;
  in >> newList >> source >> status;
  if (!in)
  {
    reportError(out, "Usage: filter_not <new_list> <source_list> <status>");
    return;
  }
  if (status != "unread" && status != "reading" && status != "finished")
  {
    reportError(out, "Invalid status");
    return;
  }
  if (core_.filterNotStatus(newList, source, status))
  {
    out << "List " << newList << " created as books not having status " << status << "\n";
  }
  else
  {
    reportError(out, "Filtering error (source list is not personal or new list already exists)");
  }
}

void velizade::CommandDispatcher::cmdMerge(std::istream& in, std::ostream& out)
{
  std::string newList, l1, l2;
  in >> newList >> l1 >> l2;
  if (!in)
  {
    reportError(out, "Usage: merge <new_list> <list1> <list2>");
    return;
  }
  if (core_.mergeLists(newList, l1, l2))
  {
    out << "List " << newList << " created as union of " << l1 << " and " << l2 << "\n";
  }
  else
  {
    reportError(out, "Merge error");
  }
}

void velizade::CommandDispatcher::cmdUnion(std::istream& in, std::ostream& out)
{
  std::string newList, l1, l2;
  in >> newList >> l1 >> l2;
  if (!in)
  {
    reportError(out, "Usage: union <new_list> <list1> <list2>");
    return;
  }
  if (core_.intersectLists(newList, l1, l2))
  {
    out << "List " << newList << " created as intersection of " << l1 << " and " << l2 << "\n";
  }
  else
  {
    reportError(out, "Intersection error");
  }
}

void velizade::CommandDispatcher::cmdComplement(std::istream& in, std::ostream& out)
{
  std::string newList, l1, l2;
  in >> newList >> l1 >> l2;
  if (!in)
  {
    reportError(out, "Usage: complement <new_list> <list1> <list2>");
    return;
  }
  if (core_.complementLists(newList, l1, l2))
  {
    out << "List " << newList << " created as difference of " << l1 << " and " << l2 << "\n";
  }
  else
  {
    reportError(out, "Complement error");
  }
}

void velizade::CommandDispatcher::cmdExport(std::istream& in, std::ostream& out)
{
  std::string filename;
  in >> filename;
  if (!in)
  {
    reportError(out, "Usage: export <filename>");
    return;
  }
  if (core_.exportData(filename))
  {
    out << "Exported\n";
  }
  else
  {
    reportError(out, "Failed to save file");
  }
}

void velizade::CommandDispatcher::cmdImport(std::istream& in, std::ostream& out)
{
  std::string filename;
  in >> filename;
  if (!in)
  {
    reportError(out, "Usage: import <filename>");
    return;
  }
  if (core_.importData(filename))
  {
    out << "Imported\n";
  }
  else
  {
    reportError(out, "File not found");
  }
}
