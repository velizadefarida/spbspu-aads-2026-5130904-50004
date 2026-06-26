#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include "node.hpp"
#include "liter.hpp"
#include "lciter.hpp"

namespace velizade
{
  template< class T >
  class List
  {
    friend class LIter< T >;
    friend class LCIter< T >;

  private:
    Node< T >* head;
    size_t size_;

  public:
    List() noexcept;
    ~List();

    void clear();
    List(const List& other);
    List(List&& other) noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;
    void swap(List& other) noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front();
    void push_back(const T& value);
    void push_back(T&& value);

    LIter< T > insert_after(LCIter< T > pos, const T& value);
    LIter< T > insert_after(LCIter< T > pos, T&& value);
    LIter< T > erase_after(LCIter< T > pos);
    void reverse();

    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;
  };

  template< class T >
  List< T >::List() noexcept :
      head(new Node< T >()),
      size_(0)
  {}

  template< class T >
  List< T >::~List()
  {
    clear();
    delete head;
  }

  template< class T >
  void List< T >::clear()
  {
    Node< T >* cur = head->next;
    while (cur)
    {
      Node< T >* tmp = cur;
      cur = cur->next;
      delete tmp;
    }
    head->next = nullptr;
    size_ = 0;
  }

  template< class T >
  List< T >::List(const List& other) :
      head(new Node< T >()),
      size_(0)
  {
    try
    {
      Node< T >* cur = head;
      for (Node< T >* src = other.head->next; src; src = src->next)
      {
        cur->next = new Node< T >(src->data);
        cur = cur->next;
        ++size_;
      }
    }
    catch (...)
    {
      clear();
      delete head;
      throw;
    }
  }

  template< class T >
  List< T >::List(List&& other) noexcept :
      head(other.head),
      size_(other.size_)
  {
    other.head = new Node< T >();
    other.size_ = 0;
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != &other)
    {
      List tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != &other)
    {
      clear();
      delete head;
      head = other.head;
      size_ = other.size_;
      other.head = new Node< T >();
      other.size_ = 0;
    }
    return *this;
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(head, other.head);
    std::swap(size_, other.size_);
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return head->next == nullptr;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  T& List< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("List is empty");
    }
    return head->next->data;
  }

  template< class T >
  const T& List< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("List is empty");
    }
    return head->next->data;
  }

  template< class T >
  T& List< T >::back()
  {
    if (empty())
    {
      throw std::runtime_error("List is empty");
    }
    Node< T >* cur = head;
    while (cur->next)
    {
      cur = cur->next;
    }
    return cur->data;
  }

  template< class T >
  const T& List< T >::back() const
  {
    if (empty())
    {
      throw std::runtime_error("List is empty");
    }
    Node< T >* cur = head;
    while (cur->next)
    {
      cur = cur->next;
    }
    return cur->data;
  }

  template< class T >
  void List< T >::push_front(const T& value)
  {
    head->next = new Node< T >(value, head->next);
    ++size_;
  }

  template< class T >
  void List< T >::push_front(T&& value)
  {
    head->next = new Node< T >(std::move(value), head->next);
    ++size_;
  }

  template< class T >
  void List< T >::pop_front()
  {
    if (empty())
    {
      throw std::runtime_error("List is empty");
    }
    Node< T >* to_delete = head->next;
    head->next = to_delete->next;
    delete to_delete;
    --size_;
  }

  template< class T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* cur = head;
    while (cur->next)
    {
      cur = cur->next;
    }
    cur->next = new Node< T >(value);
    ++size_;
  }

  template< class T >
  void List< T >::push_back(T&& value)
  {
    Node< T >* cur = head;
    while (cur->next)
    {
      cur = cur->next;
    }
    cur->next = new Node< T >(std::move(value));
    ++size_;
  }

  template< class T >
  LIter< T > List< T >::insert_after(LCIter< T > pos, const T& value)
  {
    if (pos == cend())
    {
      throw std::runtime_error("Cannot insert after end");
    }
    Node< T >* node = const_cast< Node< T >* >(pos.ptr);
    node->next = new Node< T >(value, node->next);
    ++size_;
    return LIter< T >(node->next);
  }

  template< class T >
  LIter< T > List< T >::insert_after(LCIter< T > pos, T&& value)
  {
    if (pos == cend())
    {
      throw std::runtime_error("Cannot insert after end");
    }
    Node< T >* node = const_cast< Node< T >* >(pos.ptr);
    node->next = new Node< T >(std::move(value), node->next);
    ++size_;
    return LIter< T >(node->next);
  }

  template< class T >
  LIter< T > List< T >::erase_after(LCIter< T > pos)
  {
    if (pos == cend() || pos.ptr->next == nullptr)
    {
      return LIter< T >(nullptr);
    }
    Node< T >* node = const_cast< Node< T >* >(pos.ptr);
    Node< T >* to_delete = node->next;
    node->next = to_delete->next;
    delete to_delete;
    --size_;
    return LIter< T >(node->next);
  }

  template< class T >
  void List< T >::reverse()
  {
    if (empty() || head->next->next == nullptr)
    {
      return;
    }
    Node< T >* prev = nullptr;
    Node< T >* cur = head->next;
    Node< T >* next = nullptr;
    while (cur)
    {
      next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
    head->next = prev;
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(head);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(head);
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(nullptr);
  }
}

#endif
