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
    List() noexcept :
        head(new Node< T >()),
        size_(0)
    {}

    ~List()
    {
      clear();
      delete head;
    }

    void clear()
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

    List(const List& other) :
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

    List(List&& other) noexcept :
        head(other.head),
        size_(other.size_)
    {
      other.head = new Node< T >();
      other.size_ = 0;
    }

    List& operator=(const List& other)
    {
      if (this != &other)
      {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
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

    void swap(List& other) noexcept
    {
      std::swap(head, other.head);
      std::swap(size_, other.size_);
    }

    bool empty() const noexcept
    {
      return head->next == nullptr;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    T& front()
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      return head->next->data;
    }

    const T& front() const
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      return head->next->data;
    }

    T& back()
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

    const T& back() const
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

    void push_front(const T& value)
    {
      head->next = new Node< T >(value, head->next);
      ++size_;
    }

    void push_front(T&& value)
    {
      head->next = new Node< T >(std::move(value), head->next);
      ++size_;
    }

    void pop_front()
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

    void push_back(const T& value)
    {
      Node<T>* cur = head;
      while (cur->next)
      {
        cur = cur->next;
      }
      cur->next = new Node<T>(value);
      ++size_;
    }

    void push_back(T&& value)
    {
      Node<T>* cur = head;
      while (cur->next)
      {
        cur = cur->next;
      }
      cur->next = new Node<T>(std::move(value));
      ++size_;
    }

    LIter< T > insert_after(LCIter< T > pos, const T& value)
    {
      if (pos == cend())
      {
        throw std::runtime_error("Cannot insert after end");
      }
      Node< T >* node = const_cast<Node< T >*>(pos.ptr);
      node->next = new Node< T >(value, node->next);
      ++size_;
      return LIter< T >(node->next);
    }

    LIter< T > insert_after(LCIter< T > pos, T&& value)
    {
      if (pos == cend())
      {
        throw std::runtime_error("Cannot insert after end");
      }
      Node< T >* node = const_cast<Node< T >*>(pos.ptr);
      node->next = new Node< T >(std::move(value), node->next);
      ++size_;
      return LIter< T >(node->next);
    }

    LIter< T > erase_after(LCIter< T > pos)
    {
      if (pos == cend() || pos.ptr->next == nullptr)
      {
        return LIter< T >(nullptr);
      }
      Node< T >* node = const_cast<Node< T >*>(pos.ptr);
      Node< T >* to_delete = node->next;
      node->next = to_delete->next;
      delete to_delete;
      --size_;
      return LIter< T >(node->next);
    }

    void reverse()
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

    LIter< T > begin() noexcept
    {
      return LIter< T >(head);
    }

    LIter< T > end() noexcept
    {
      return LIter< T >(nullptr);
    }

    LCIter< T > cbegin() const noexcept
    {
      return LCIter< T >(head);
    }

    LCIter< T > cend() const noexcept
    {
      return LCIter< T >(nullptr);
    }
  };
}

#endif
