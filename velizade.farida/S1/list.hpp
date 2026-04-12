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
    LIter<T> begin() noexcept { return LIter<T>(head); }
    LIter<T> end() noexcept { return LIter<T>(nullptr); }
    LCIter<T> cbegin() const noexcept { return LCIter<T>(head); }
    LCIter<T> cend() const noexcept { return LCIter<T>(nullptr); }
  };
}

#endif
