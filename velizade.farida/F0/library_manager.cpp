#include "library_manager.hpp"
#include <fstream>
#include <algorithm>
#include <cstddef>
#include <stdexcept>

bool velizade::LibraryManager::isPersonal(const std::string& name) const
{
  return personalLists.find(name);
}

bool velizade::LibraryManager::isDerived(const std::string& name) const
{
  return derivedLists.find(name);
}

bool velizade::LibraryManager::listExists(const std::string& name) const
{
  return isPersonal(name) || isDerived(name);
}

AVLTree<int, velizade::Status>* velizade::LibraryManager::getPersonalListPtr(const std::string& name)
{
  AVLTree<int, Status>* ptr = personalLists.findPtr(name);
  if (!ptr)
  {
    AVLTree<int, Status> empty;
    personalLists.insert(name, empty);
    ptr = personalLists.findPtr(name);
  }
  return ptr;
}

AVLTree<int, bool>* velizade::LibraryManager::getDerivedListPtr(const std::string& name)
{
  AVLTree<int, bool>* ptr = derivedLists.findPtr(name);
  if (!ptr)
  {
    AVLTree<int, bool> empty;
    derivedLists.insert(name, empty);
    ptr = derivedLists.findPtr(name);
  }
  return ptr;
}

void velizade::LibraryManager::collectKeys(const std::string& name, Vector<int>& keys) const
{
  if (isPersonal(name))
  {
    const AVLTree<int, Status>* list = personalLists.findPtr(name);
    if (list)
    {
      for (auto it = list->begin(); it != list->end(); ++it)
      {
        keys.pushBack((*it).first);
      }
    }
  }
  else if (isDerived(name))
  {
    const AVLTree<int, bool>* list = derivedLists.findPtr(name);
    if (list)
    {
      for (auto it = list->begin(); it != list->end(); ++it)
      {
        keys.pushBack((*it).first);
      }
    }
  }
}

bool velizade::LibraryManager::listList(const std::string& listName, std::ostream& out) const
{
  if (isPersonal(listName))
  {
    const AVLTree<int, Status>* list = personalLists.findPtr(listName);
    if (!list)
    {
      return false;
    }
    out << "List " << listName << " (";
    size_t count = 0;
    for (auto it = list->begin(); it != list->end(); ++it)
    {
      ++count;
      int id = (*it).first;
      Status status = (*it).second;
      Book book;
      if (globalCatalog.find(id, book))
      {
        out << id << " " << book.author << " " << book.title << " " << book.year
            << " Status: " << statusToString(status) << "\n";
      }
    }
    out << count << " books)\n";
    return true;
  }
  if (isDerived(listName))
  {
    const AVLTree<int, bool>* list = derivedLists.findPtr(listName);
    if (!list)
    {
      return false;
    }
    out << "List " << listName << " (";
    size_t count = 0;
    for (auto it = list->begin(); it != list->end(); ++it)
    {
      ++count;
      int id = (*it).first;
      Book book;
      if (globalCatalog.find(id, book))
      {
        out << id << " " << book.author << " " << book.title << " " << book.year << "\n";
      }
    }
    out << count << " books)\n";
    return true;
  }
  return false;
}

bool velizade::LibraryManager::addBook(int id, const std::string& author, const std::string& title, int year)
{
  Book b{author, title, year};
  return globalCatalog.insert(id, b).second;
}

bool velizade::LibraryManager::showBook(int id, Book& book) const
{
  return globalCatalog.find(id, book);
}

bool velizade::LibraryManager::addBookToList(const std::string& listName, int bookId)
{
  Book dummy;
  if (!globalCatalog.find(bookId, dummy))
  {
    return false;
  }
  if (isDerived(listName))
  {
    return false;
  }
  AVLTree<int, Status>* list = getPersonalListPtr(listName);
  if (list->find(bookId))
  {
    return false;
  }
  list->insert(bookId, Status::UNREAD);
  return true;
}

bool velizade::LibraryManager::removeFromList(const std::string& listName, int bookId)
{
  if (isPersonal(listName))
  {
    AVLTree<int, Status>* list = getPersonalListPtr(listName);
    if (!list->find(bookId))
    {
      return false;
    }
    list->remove(bookId);
    return true;
  }
  if (isDerived(listName))
  {
    AVLTree<int, bool>* list = getDerivedListPtr(listName);
    if (!list->find(bookId))
    {
      return false;
    }
    list->remove(bookId);
    return true;
  }
  return false;
}

bool velizade::LibraryManager::setStatus(const std::string& listName, int bookId, const std::string& statusStr)
{
  if (!isPersonal(listName))
  {
    return false;
  }
  AVLTree<int, Status>* list = getPersonalListPtr(listName);
  if (!list->find(bookId))
  {
    return false;
  }
  Status s = stringToStatus(statusStr);
  list->remove(bookId);
  list->insert(bookId, s);
  return true;
}

bool velizade::LibraryManager::filterByStatus(const std::string& newList, const std::string& sourceList, const std::string& statusStr)
{
  if (!isPersonal(sourceList))
  {
    return false;
  }
  if (listExists(newList))
  {
    return false;
  }
  Status s = stringToStatus(statusStr);
  const AVLTree<int, Status>* src = personalLists.findPtr(sourceList);
  if (!src)
  {
    return false;
  }
  for (auto it = src->begin(); it != src->end(); ++it)
  {
    if ((*it).second == s)
    {
      getDerivedListPtr(newList)->insert((*it).first, true);
    }
  }
  return true;
}

bool velizade::LibraryManager::filterNotStatus(const std::string& newList, const std::string& sourceList, const std::string& statusStr)
{
  if (!isPersonal(sourceList))
  {
    return false;
  }
  if (listExists(newList))
  {
    return false;
  }
  Status s = stringToStatus(statusStr);
  const AVLTree<int, Status>* src = personalLists.findPtr(sourceList);
  if (!src)
  {
    return false;
  }
  for (auto it = src->begin(); it != src->end(); ++it)
  {
    if ((*it).second != s)
    {
      getDerivedListPtr(newList)->insert((*it).first, true);
    }
  }
  return true;
}

bool velizade::LibraryManager::mergeLists(const std::string& newList, const std::string& l1, const std::string& l2)
{
  if (!listExists(l1) || !listExists(l2))
  {
    return false;
  }
  if (listExists(newList))
  {
    return false;
  }
  Vector<int> keys;
  collectKeys(l1, keys);
  collectKeys(l2, keys);
  std::sort(keys.begin(), keys.end());
  keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

  AVLTree<int, bool> newTree;
  for (auto it = keys.begin(); it != keys.end(); ++it)
  {
    if (!newTree.insert(*it, true).second)
    {
      throw std::runtime_error("Failed to insert into new tree");
    }
  }
  derivedLists.insert(newList, std::move(newTree));
  return true;
}

bool velizade::LibraryManager::intersectLists(const std::string& newList, const std::string& l1, const std::string& l2)
{
  if (!listExists(l1) || !listExists(l2))
  {
    return false;
  }
  if (listExists(newList))
  {
    return false;
  }
  Vector<int> keys1, keys2;
  collectKeys(l1, keys1);
  collectKeys(l2, keys2);
  std::sort(keys1.begin(), keys1.end());
  std::sort(keys2.begin(), keys2.end());

  Vector<int> common;
  size_t i = 0, j = 0;
  while (i < keys1.getSize() && j < keys2.getSize())
  {
    if (keys1[i] < keys2[j])
    {
      ++i;
    }
    else if (keys1[i] > keys2[j])
    {
      ++j;
    }
    else
    {
      common.pushBack(keys1[i]);
      ++i;
      ++j;
    }
  }

  AVLTree<int, bool> newTree;
  for (auto it = common.begin(); it != common.end(); ++it)
  {
    if (!newTree.insert(*it, true).second)
    {
      throw std::runtime_error("Failed to insert into new tree");
    }
  }
  derivedLists.insert(newList, std::move(newTree));
  return true;
}

bool velizade::LibraryManager::complementLists(const std::string& newList, const std::string& l1, const std::string& l2)
{
  if (!listExists(l1) || !listExists(l2))
  {
    return false;
  }
  if (listExists(newList))
  {
    return false;
  }
  Vector<int> keys1, keys2;
  collectKeys(l1, keys1);
  collectKeys(l2, keys2);
  std::sort(keys1.begin(), keys1.end());
  std::sort(keys2.begin(), keys2.end());

  Vector<int> diff;
  size_t i = 0, j = 0;
  while (i < keys1.getSize())
  {
    if (j < keys2.getSize() && keys1[i] > keys2[j])
    {
      ++j;
    }
    else if (j < keys2.getSize() && keys1[i] == keys2[j])
    {
      ++i;
      ++j;
    }
    else
    {
      diff.pushBack(keys1[i]);
      ++i;
    }
  }

  AVLTree<int, bool> newTree;
  for (auto it = diff.begin(); it != diff.end(); ++it)
  {
    if (!newTree.insert(*it, true).second)
    {
      throw std::runtime_error("Failed to insert into new tree");
    }
  }
  derivedLists.insert(newList, std::move(newTree));
  return true;
}

bool velizade::LibraryManager::exportData(const std::string& filename) const
{
  std::ofstream fout(filename);
  if (!fout)
  {
    return false;
  }

  auto writeBooks = [&](const AVLTree<int, Book>& tree)
  {
    fout << "Books:\n";
    for (auto it = tree.begin(); it != tree.end(); ++it)
    {
      fout << (*it).first << "|" << (*it).second.author << "|"
           << (*it).second.title << "|" << (*it).second.year << "\n";
    }
  };
  writeBooks(globalCatalog);

  fout << "Personal lists:\n";
  for (auto it = personalLists.begin(); it != personalLists.end(); ++it)
  {
    fout << "List " << (*it).first << "\n";
    for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
    {
      fout << (*it2).first << "|" << statusToString((*it2).second) << "\n";
    }
  }

  fout << "Derived lists:\n";
  for (auto it = derivedLists.begin(); it != derivedLists.end(); ++it)
  {
    fout << "List " << (*it).first << "\n";
    for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
    {
      fout << (*it2).first << "\n";
    }
  }

  fout.close();
  return true;
}

bool velizade::LibraryManager::importData(const std::string& filename)
{
  std::ifstream fin(filename);
  if (!fin)
  {
    return false;
  }

  globalCatalog.clear();
  personalLists.clear();
  derivedLists.clear();

  std::string line;
  std::string section;
  while (std::getline(fin, line))
  {
    if (line.empty())
    {
      continue;
    }
    if (line == "Books:")
    {
      section = "books";
      continue;
    }
    else if (line == "Personal lists:")
    {
      section = "personal";
      continue;
    }
    else if (line == "Derived lists:")
    {
      section = "derived";
      continue;
    }
    if (section == "books")
    {
      size_t pos1 = line.find('|');
      size_t pos2 = line.find('|', pos1 + 1);
      size_t pos3 = line.find('|', pos2 + 1);
      int id = std::stoi(line.substr(0, pos1));
      std::string author = line.substr(pos1 + 1, pos2 - pos1 - 1);
      std::string title = line.substr(pos2 + 1, pos3 - pos2 - 1);
      int year = std::stoi(line.substr(pos3 + 1));
      globalCatalog.insert(id, Book{author, title, year});
    }
    else if (section == "personal")
    {
      if (line.find("List ") == 0)
      {
        std::string name = line.substr(5);
        AVLTree<int, Status> list;
        while (std::getline(fin, line) && !line.empty() && line.find("List ") != 0 && line != "Derived lists:")
        {
          size_t pos = line.find('|');
          int id = std::stoi(line.substr(0, pos));
          Status s = stringToStatus(line.substr(pos + 1));
          list.insert(id, s);
        }
        personalLists.insert(name, std::move(list));
      }
    }
    else if (section == "derived")
    {
      if (line.find("List ") == 0)
      {
        std::string name = line.substr(5);
        AVLTree<int, bool> list;
        while (std::getline(fin, line) && !line.empty() && line.find("List ") != 0 && line != "Books:")
        {
          int id = std::stoi(line);
          list.insert(id, true);
        }
        derivedLists.insert(name, std::move(list));
      }
    }
  }
  fin.close();
  return true;
}
