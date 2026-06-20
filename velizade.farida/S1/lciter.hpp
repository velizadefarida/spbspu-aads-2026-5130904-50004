#ifndef LCITER_HPP
#define LCITER_HPP

#include <iterator>
#include "node.hpp"

namespace velizade
{
  template< class T >
  class List;

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
    const Node< T >* ptr;
    explicit LCIter(const Node< T >* p) noexcept : ptr(p) {}
  };
}

#endif
