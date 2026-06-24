#include "library_manager.hpp"
#include <fstream>
#include <algorithm>
#include <cstddef>

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

velizade::AVLTree<int, velizade::Status>* velizade::LibraryManager::getPersonalListPtr(const std::string& name)
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

velizade::AVLTree<int, bool>* velizade::LibraryManager::getDerivedListPtr(const std::string& name)
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

bool velizade::LibraryManager::addBook(int id, const std::string& author, const std::string& title, int year)
{
  Book b{author, title, year};
  return globalCatalog.insert(id, b);
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

bool velizade::LibraryManager::listList(const std::string& listName, Vector<std::string>& output) const
{
  if (isPersonal(listName))
  {
    AVLTree<int, Status>* list = personalLists.findPtr(listName);
    if (!list)
    {
      return false;
    }
    auto entries = list->getAll();
    output.push_back("List " + listName + " (" + std::to_string(entries.getSize()) + " books):");
    for (size_t i = 0; i < entries.getSize(); ++i)
    {
      const auto& p = entries[i];
      Book book;
      if (globalCatalog.find(p.first, book))
      {
        output.push_back(std::to_string(p.first) + " " + book.author + " " + book.title + " " + std::to_string(book.year) + " Status: " + statusToString(p.second));
      }
    }
    return true;
  }
  if (isDerived(listName))
  {
    AVLTree<int, bool>* list = derivedLists.findPtr(listName);
    if (!list)
    {
      return false;
    }
    auto entries = list->getAll();
    output.push_back("List " + listName + " (" + std::to_string(entries.getSize()) + " books):");
    for (size_t i = 0; i < entries.getSize(); ++i)
    {
      const auto& p = entries[i];
      Book book;
      if (globalCatalog.find(p.first, book))
      {
        output.push_back(std::to_string(p.first) + " " + book.author + " " + book.title + " " + std::to_string(book.year));
      }
    }
    return true;
  }
  return false;
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
  AVLTree<int, Status>* src = getPersonalListPtr(sourceList);
  auto entries = src->getAll();
  for (size_t i = 0; i < entries.getSize(); ++i)
  {
    const auto& p = entries[i];
    if (p.second == s)
    {
      getDerivedListPtr(newList)->insert(p.first, true);
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
  AVLTree<int, Status>* src = getPersonalListPtr(sourceList);
  auto entries = src->getAll();
  for (size_t i = 0; i < entries.getSize(); ++i)
  {
    const auto& p = entries[i];
    if (p.second != s)
    {
      getDerivedListPtr(newList)->insert(p.first, true);
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
  auto addKeys = [&](const std::string& name)
  {
    if (isPersonal(name))
    {
      auto entries = getPersonalListPtr(name)->getAll();
      for (size_t i = 0; i < entries.getSize(); ++i)
      {
        keys.push_back(entries[i].first);
      }
    }
    else
    {
      auto entries = getDerivedListPtr(name)->getAll();
      for (size_t i = 0; i < entries.getSize(); ++i)
      {
        keys.push_back(entries[i].first);
      }
    }
  };
  addKeys(l1);
  addKeys(l2);
  for (size_t i = 0; i < keys.getSize(); ++i)
  {
    for (size_t j = i + 1; j < keys.getSize(); ++j)
    {
      if (keys[i] > keys[j])
      {
        int tmp = keys[i];
        keys[i] = keys[j];
        keys[j] = tmp;
      }
    }
  }
  if (keys.getSize() > 0)
  {
    size_t write = 1;
    for (size_t read = 1; read < keys.getSize(); ++read)
    {
      if (keys[read] != keys[write - 1])
      {
        keys[write++] = keys[read];
      }
    }
    Vector<int> unique;
    for (size_t i = 0; i < write; ++i)
    {
      unique.push_back(keys[i]);
    }
    keys = unique;
  }
  AVLTree<int, bool>* newListTree = getDerivedListPtr(newList);
  for (size_t i = 0; i < keys.getSize(); ++i)
  {
    newListTree->insert(keys[i], true);
  }
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
  Vector<int> keys1;
  if (isPersonal(l1))
  {
    auto entries = getPersonalListPtr(l1)->getAll();
    for (size_t i = 0; i < entries.getSize(); ++i)
    {
      keys1.push_back(entries[i].first);
    }
  }
  else
  {
    auto entries = getDerivedListPtr(l1)->getAll();
    for (size_t i = 0; i < entries.getSize(); ++i)
    {
      keys1.push_back(entries[i].first);
    }
  }
  for (size_t i = 0; i < keys1.getSize(); ++i)
  {
    int id = keys1[i];
    bool found = false;
    if (isPersonal(l2))
    {
      found = getPersonalListPtr(l2)->find(id);
    }
    else
    {
      found = getDerivedListPtr(l2)->find(id);
    }
    if (found)
    {
      getDerivedListPtr(newList)->insert(id, true);
    }
  }
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
  Vector<int> keys1;
  if (isPersonal(l1))
  {
    auto entries = getPersonalListPtr(l1)->getAll();
    for (size_t i = 0; i < entries.getSize(); ++i)
    {
      keys1.push_back(entries[i].first);
    }
  }
  else
  {
    auto entries = getDerivedListPtr(l1)->getAll();
    for (size_t i = 0; i < entries.getSize(); ++i)
    {
      keys1.push_back(entries[i].first);
    }
  }
  for (size_t i = 0; i < keys1.getSize(); ++i)
  {
    int id = keys1[i];
    bool found = false;
    if (isPersonal(l2))
    {
      found = getPersonalListPtr(l2)->find(id);
    }
    else
    {
      found = getDerivedListPtr(l2)->find(id);
    }
    if (!found)
    {
      getDerivedListPtr(newList)->insert(id, true);
    }
  }
  return true;
}

bool velizade::LibraryManager::exportData(const std::string& filename) const
{
  std::ofstream fout(filename);
  if (!fout)
  {
    return false;
  }
  auto books = globalCatalog.getAll();
  fout << books.getSize() << "\n";
  for (size_t i = 0; i < books.getSize(); ++i)
  {
    const auto& p = books[i];
    fout << p.first << "|" << p.second.author << "|" << p.second.title << "|" << p.second.year << "\n";
  }

  auto personalNames = personalLists.getAll();
  fout << personalNames.getSize() << "\n";
  for (size_t i = 0; i < personalNames.getSize(); ++i)
  {
    const auto& nameEntry = personalNames[i];
    const std::string& name = nameEntry.first;
    const AVLTree<int, Status>& list = nameEntry.second;
    auto entries = list.getAll();
    fout << name << "|" << entries.getSize() << "\n";
    for (size_t j = 0; j < entries.getSize(); ++j)
    {
      const auto& e = entries[j];
      fout << e.first << "|" << statusToString(e.second) << "\n";
    }
  }

  auto derivedNames = derivedLists.getAll();
  fout << derivedNames.getSize() << "\n";
  for (size_t i = 0; i < derivedNames.getSize(); ++i)
  {
    const auto& nameEntry = derivedNames[i];
    const std::string& name = nameEntry.first;
    const AVLTree<int, bool>& list = nameEntry.second;
    auto entries = list.getAll();
    fout << name << "|" << entries.getSize() << "\n";
    for (size_t j = 0; j < entries.getSize(); ++j)
    {
      fout << entries[j].first << "\n";
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

  size_t cnt;
  fin >> cnt; fin.ignore();
  for (size_t i = 0; i < cnt; ++i)
  {
    std::string line;
    std::getline(fin, line);
    size_t pos = 0;
    std::string parts[4];
    for (int j = 0; j < 4; ++j)
    {
      size_t next = line.find('|', pos);
      parts[j] = line.substr(pos, next - pos);
      pos = next + 1;
    }
    int id = std::stoi(parts[0]);
    int year = std::stoi(parts[3]);
    globalCatalog.insert(id, {parts[1], parts[2], year});
  }

  fin >> cnt; fin.ignore();
  for (size_t i = 0; i < cnt; ++i)
  {
    std::string line;
    std::getline(fin, line);
    size_t pos = line.find('|');
    std::string name = line.substr(0, pos);
    size_t entries = std::stoul(line.substr(pos + 1));
    AVLTree<int, Status> list;
    for (size_t j = 0; j < entries; ++j)
    {
      std::getline(fin, line);
      pos = line.find('|');
      int id = std::stoi(line.substr(0, pos));
      std::string statusStr = line.substr(pos + 1);
      Status s = stringToStatus(statusStr);
      list.insert(id, s);
    }
    personalLists.insert(name, list);
  }

  fin >> cnt; fin.ignore();
  for (size_t i = 0; i < cnt; ++i)
  {
    std::string line;
    std::getline(fin, line);
    size_t pos = line.find('|');
    std::string name = line.substr(0, pos);
    size_t entries = std::stoul(line.substr(pos + 1));
    AVLTree<int, bool> list;
    for (size_t j = 0; j < entries; ++j)
    {
      std::getline(fin, line);
      int id = std::stoi(line);
      list.insert(id, true);
    }
    derivedLists.insert(name, list);
  }
  fin.close();
  return true;
}
