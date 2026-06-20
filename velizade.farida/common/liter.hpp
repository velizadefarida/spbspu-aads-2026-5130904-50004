#ifndef LITER_HPP
#define LITER_HPP

#include <iterator>
#include "node.hpp"

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
    Node< T >* ptr;
    explicit LIter(Node< T >* p) noexcept : ptr(p) {}
  };
}

#endif
