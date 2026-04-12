#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace velizade
{
  template< class T >
  class List
  {
  private:
    struct Node
    {
      T data;
      Node* next;

      explicit Node(const T& val, Node* n = nullptr) : data(val), next(n) {}
      explicit Node(T&& val, Node* n = nullptr) : data(std::move(val)), next(n) {}
    };

    Node* head;
    size_t size_;

  public:
    List() noexcept : head(nullptr), size_(0) {}

    ~List()
    {
      clear();
    }

    void clear()
    {
      while (head)
      {
        Node* tmp = head;
        head = head->next;
        delete tmp;
      }
      size_ = 0;
    }
  };
}

#endif
