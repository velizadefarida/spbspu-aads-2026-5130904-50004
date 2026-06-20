#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace velizade
{
  template< class T >
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
}

#endif
