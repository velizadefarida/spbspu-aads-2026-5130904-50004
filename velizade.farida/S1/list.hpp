#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace velizade
{
  template< class T >
  class List;

  template< class T >
  class LIter : public std::iterator<std::forward_iterator_tag, T>
  {
    friend class List< T >;

  public:
    LIter() noexcept : ptr(nullptr) {}

    LIter(const LIter&) noexcept = default;
    LIter(LIter&&) noexcept = default;
    ~LIter() = default;

    LIter& operator=(const LIter&) noexcept = default;
    LIter& operator=(LIter&&) noexcept = default;

    T& operator*() const noexcept
    {
      return ptr->data;
    }

    T* operator->() const noexcept
    {
      return &(ptr->data);
    }

    LIter& operator++() noexcept
    {
      ptr = ptr->next;
      return *this;
    }

    LIter operator++(int) noexcept
    {
      LIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LIter& other) const noexcept
    {
      return ptr == other.ptr;
    }

    bool operator!=(const LIter& other) const noexcept
    {
      return ptr != other.ptr;
    }

  private:
    typename List< T >::Node* ptr;
    explicit LIter(typename List< T >::Node* p) noexcept : ptr(p) {}
  };

  template< class T >
  class LCIter : public std::iterator<std::forward_iterator_tag, const T>
  {
    friend class List< T >;

  public:
    LCIter() noexcept : ptr(nullptr) {}

    LCIter(const LCIter&) noexcept = default;
    LCIter(LCIter&&) noexcept = default;
    ~LCIter() = default;

    LCIter& operator=(const LCIter&) noexcept = default;
    LCIter& operator=(LCIter&&) noexcept = default;

    const T& operator*() const noexcept
    {
      return ptr->data;
    }

    const T* operator->() const noexcept
    {
      return &(ptr->data);
    }

    LCIter& operator++() noexcept
    {
      ptr = ptr->next;
      return *this;
    }

    LCIter operator++(int) noexcept
    {
      LCIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LCIter& other) const noexcept
    {
      return ptr == other.ptr;
    }

    bool operator!=(const LCIter& other) const noexcept
    {
      return ptr != other.ptr;
    }

  private:
    const typename List< T >::Node* ptr;
    explicit LCIter(const typename List< T >::Node* p) noexcept : ptr(p) {}
  };

  template< class T >
  class List
  {
    friend class LIter< T >;
    friend class LCIter< T >;

  private:
    struct Node
    {
      T data;
      Node* next;

      explicit Node(const T& val, Node* n = nullptr) :
          data(val),
          next(n)
      {}

      explicit Node(T&& val, Node* n = nullptr) :
          data(std::move(val)),
          next(n)
      {}

      Node() noexcept :
          next(nullptr)
      {}
    };

    Node* head;
    size_t size_;

  public:
    List() noexcept :
        head(new Node()),
        size_(0)
    {}

    ~List()
    {
      clear();
      delete head;
    }

    void clear()
    {
      Node* cur = head->next;
      while (cur)
      {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
      }
      head->next = nullptr;
      size_ = 0;
    }

    List(const List& other) :
        head(new Node()),
        size_(0)
    {
      try
      {
        Node* cur = head;
        for (Node* src = other.head->next; src; src = src->next)
        {
          cur->next = new Node(src->data);
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
      other.head = new Node();
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
        other.head = new Node();
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
      Node* cur = head;
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
      Node* cur = head;
      while (cur->next)
      {
        cur = cur->next;
      }
      return cur->data;
    }

    void push_front(const T& value)
    {
      head->next = new Node(value, head->next);
      ++size_;
    }

    void push_front(T&& value)
    {
      head->next = new Node(std::move(value), head->next);
      ++size_;
    }

    void pop_front()
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      Node* to_delete = head->next;
      head->next = to_delete->next;
      delete to_delete;
      --size_;
    }

    LIter< T > insert_after(LCIter< T > pos, const T& value)
    {
      if (pos == cend())
      {
        throw std::runtime_error("Cannot insert after end");
      }
      Node* node = const_cast<Node*>(pos.ptr);
      node->next = new Node(value, node->next);
      ++size_;
      return LIter< T >(node->next);
    }

    LIter< T > insert_after(LCIter< T > pos, T&& value)
    {
      if (pos == cend())
      {
        throw std::runtime_error("Cannot insert after end");
      }
      Node* node = const_cast<Node*>(pos.ptr);
      node->next = new Node(std::move(value), node->next);
      ++size_;
      return LIter< T >(node->next);
    }

    LIter< T > erase_after(LCIter< T > pos)
    {
      if (pos == cend() || pos.ptr->next == nullptr)
      {
        return LIter< T >(nullptr);
      }
      Node* node = const_cast<Node*>(pos.ptr);
      Node* to_delete = node->next;
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
      Node* prev = nullptr;
      Node* cur = head->next;
      Node* next = nullptr;
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
