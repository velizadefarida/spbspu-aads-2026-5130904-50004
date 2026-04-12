#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace velizade
{
  template< class T > class List;

  template< class T >
  class LIter
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
    typename List<T>::Node* ptr;
    explicit LIter(typename List<T>::Node* p) noexcept : ptr(p) {}
  };

  template< class T >
  class LCIter
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
    const typename List<T>::Node* ptr;
    explicit LCIter(const typename List<T>::Node* p) noexcept : ptr(p) {}
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

    List(const List& other) : head(nullptr), size_(0)
    {
      try
      {
        Node** cur = &head;
        for (Node* src = other.head; src; src = src->next)
        {
          *cur = new Node(src->data);
          cur = &((*cur)->next);
          ++size_;
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }

    List(List&& other) noexcept : head(other.head), size_(other.size_)
    {
      other.head = nullptr;
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
        head = other.head;
        size_ = other.size_;
        other.head = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    void swap(List& other) noexcept
    {
      Node* tmp_head = head;
      size_t tmp_size = size_;
      head = other.head;
      size_ = other.size_;
      other.head = tmp_head;
      other.size_ = tmp_size;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    T& front()
    {
      if (empty()) throw std::runtime_error("List is empty");
      return head->data;
    }

    const T& front() const
    {
      if (empty()) throw std::runtime_error("List is empty");
      return head->data;
    }

    T& back()
    {
      if (empty()) throw std::runtime_error("List is empty");
      Node* cur = head;
      while (cur->next) cur = cur->next;
      return cur->data;
    }

    const T& back() const
    {
      if (empty()) throw std::runtime_error("List is empty");
      Node* cur = head;
      while (cur->next) cur = cur->next;
      return cur->data;
    }

    void push_front(const T& value)
    {
      head = new Node(value, head);
      ++size_;
    }

    void push_front(T&& value)
    {
      head = new Node(std::move(value), head);
      ++size_;
    }

    void pop_front()
    {
      if (empty()) throw std::runtime_error("List is empty");
      Node* tmp = head;
      head = head->next;
      delete tmp;
      --size_;
    }

    LIter<T> begin() noexcept { return LIter<T>(head); }
    LIter<T> end() noexcept { return LIter<T>(nullptr); }
    LCIter<T> cbegin() const noexcept { return LCIter<T>(head); }
    LCIter<T> cend() const noexcept { return LCIter<T>(nullptr); }
  };
}

#endif
