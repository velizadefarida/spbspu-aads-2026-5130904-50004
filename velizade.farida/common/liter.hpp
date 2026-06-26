#ifndef LITER_HPP
#define LITER_HPP

#include <iterator>
#include "node.hpp"

namespace velizade
{
  template< class T >
  class List;

  template< class T >
  class LIter : public std::iterator< std::forward_iterator_tag, T >
  {
    friend class List< T >;

  public:
    LIter() noexcept;
    LIter(const LIter&) noexcept = default;
    LIter(LIter&&) noexcept = default;
    ~LIter() = default;

    LIter& operator=(const LIter&) noexcept = default;
    LIter& operator=(LIter&&) noexcept = default;

    T& operator*() const noexcept;
    T* operator->() const noexcept;
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;

    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;

  private:
    Node< T >* ptr;
    explicit LIter(Node< T >* p) noexcept;
  };

  template< class T >
  LIter< T >::LIter() noexcept :
      ptr(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(Node< T >* p) noexcept :
      ptr(p)
  {}

  template< class T >
  T& LIter< T >::operator*() const noexcept
  {
    return ptr->data;
  }

  template< class T >
  T* LIter< T >::operator->() const noexcept
  {
    return &(ptr->data);
  }

  template< class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    ptr = ptr->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const noexcept
  {
    return ptr == other.ptr;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const noexcept
  {
    return ptr != other.ptr;
  }
}

#endif
