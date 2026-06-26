#ifndef LCITER_HPP
#define LCITER_HPP

#include <iterator>
#include "node.hpp"

namespace velizade
{
  template< class T >
  class List;

  template< class T >
  class LCIter : public std::iterator< std::forward_iterator_tag, const T >
  {
    friend class List< T >;

  public:
    LCIter() noexcept;
    LCIter(const LCIter&) noexcept = default;
    LCIter(LCIter&&) noexcept = default;
    ~LCIter() = default;

    LCIter& operator=(const LCIter&) noexcept = default;
    LCIter& operator=(LCIter&&) noexcept = default;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;

    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;

  private:
    const Node< T >* ptr;
    explicit LCIter(const Node< T >* p) noexcept;
  };

  template< class T >
  LCIter< T >::LCIter() noexcept :
      ptr(nullptr)
  {}

  template< class T >
  LCIter< T >::LCIter(const Node< T >* p) noexcept :
      ptr(p)
  {}

  template< class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    return ptr->data;
  }

  template< class T >
  const T* LCIter< T >::operator->() const noexcept
  {
    return &(ptr->data);
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    ptr = ptr->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter& other) const noexcept
  {
    return ptr == other.ptr;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter& other) const noexcept
  {
    return ptr != other.ptr;
  }
}

#endif
