#ifndef LIBRARY_MANAGER_HPP
#define LIBRARY_MANAGER_HPP

#include <string>
#include <ostream>
#include "book.hpp"
#include "status.hpp"
#include "avl_tree.hpp"
#include "vector.hpp"

namespace velizade
{
  class LibraryManager
  {
  private:
    AVLTree<int, Book> globalCatalog;
    AVLTree<std::string, AVLTree<int, Status>> personalLists;
    AVLTree<std::string, AVLTree<int, bool>> derivedLists;

    bool isPersonal(const std::string& name) const;
    bool isDerived(const std::string& name) const;
    bool listExists(const std::string& name) const;

    AVLTree<int, Status>* getPersonalListPtr(const std::string& name);
    AVLTree<int, bool>* getDerivedListPtr(const std::string& name);

    void collectKeys(const std::string& name, Vector<int>& keys) const;

  public:
    bool addBook(int id, const std::string& author, const std::string& title, int year);
    bool showBook(int id, Book& book) const;
    bool addBookToList(const std::string& listName, int bookId);
    bool removeFromList(const std::string& listName, int bookId);
    bool setStatus(const std::string& listName, int bookId, const std::string& statusStr);
    bool listList(const std::string& listName, std::ostream& out) const;

    bool filterByStatus(const std::string& newList, const std::string& sourceList, const std::string& statusStr);
    bool filterNotStatus(const std::string& newList, const std::string& sourceList, const std::string& statusStr);

    bool mergeLists(const std::string& newList, const std::string& l1, const std::string& l2);
    bool intersectLists(const std::string& newList, const std::string& l1, const std::string& l2);
    bool complementLists(const std::string& newList, const std::string& l1, const std::string& l2);

    bool exportData(const std::string& filename) const;
    bool importData(const std::string& filename);
  };
}

#endif
