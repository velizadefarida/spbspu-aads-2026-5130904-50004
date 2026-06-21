#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include "node.hpp"
#include <utility>
#include <functional>

namespace velizade
{
  template <typename Key, typename Value, typename Compare>
  class BSTree;

  template<typename Key, typename Value>
  class BSTIterator
  {
    friend class BSTree<Key, Value, std::less<Key>>;

  private:
    using NodePtr = Node<Key, Value>*;
    NodePtr current;
    NodePtr fake;

  public:
    using value_type = std::pair<const Key, Value>;
    using reference = value_type&;

    BSTIterator();
    explicit BSTIterator(NodePtr ptr, NodePtr fk = nullptr);

    std::pair<const Key&, Value&> operator*() const;
    const Key& key() const;
    Value& value() const;

    BSTIterator& operator++();
    BSTIterator operator++(int);
    BSTIterator& operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator& other) const;
    bool operator!=(const BSTIterator& other) const;

    NodePtr getNode() const;
  };

  template<typename Key, typename Value>
  class BSTConstIterator
  {
    using NodePtr = Node<Key, Value>*;
    NodePtr current;
    NodePtr fake;

  public:
    using value_type = std::pair<const Key, const Value>;

    BSTConstIterator();
    explicit BSTConstIterator(NodePtr ptr, NodePtr fk = nullptr);
    BSTConstIterator(const BSTIterator<Key, Value>& it);

    std::pair<const Key&, const Value&> operator*() const;
    const Key& key() const;
    const Value& value() const;

    BSTConstIterator& operator++();
    BSTConstIterator operator++(int);

    bool operator==(const BSTConstIterator& other) const;
    bool operator!=(const BSTConstIterator& other) const;

    NodePtr getNode() const;
  };

  template<typename Key, typename Value>
  BSTIterator<Key, Value>::BSTIterator()
    : current(nullptr), fake(nullptr)
  {}

  template<typename Key, typename Value>
  BSTIterator<Key, Value>::BSTIterator(NodePtr ptr, NodePtr fk)
    : current(ptr), fake(fk)
  {}

  template<typename Key, typename Value>
  std::pair<const Key&, Value&> BSTIterator<Key, Value>::operator*() const
  {
    return {current->key, current->value};
  }

  template<typename Key, typename Value>
  const Key& BSTIterator<Key, Value>::key() const
  {
    return current->key;
  }

  template<typename Key, typename Value>
  Value& BSTIterator<Key, Value>::value() const
  {
    return current->value;
  }

  template<typename Key, typename Value>
  BSTIterator<Key, Value>& BSTIterator<Key, Value>::operator++()
  {
    if (current == fake)
    {
      return *this;
    }
    if (current->right != fake)
    {
      current = current->right;
      while (current->left != fake)
      {
        current = current->left;
      }
    }
    else
    {
      NodePtr p = current->parent;
      while (p != nullptr && current == p->right)
      {
        current = p;
        p = p->parent;
      }
      current = (p == nullptr ? fake : p);
    }
    return *this;
  }

  template<typename Key, typename Value>
  BSTIterator<Key, Value> BSTIterator<Key, Value>::operator++(int)
  {
    BSTIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template<typename Key, typename Value>
  BSTIterator<Key, Value>& BSTIterator<Key, Value>::operator--()
  {
    if (current == fake)
    {
      return *this;
    }
    if (current->left != fake)
    {
      current = current->left;
      while (current->right != fake)
      {
        current = current->right;
      }
    }
    else
    {
      NodePtr p = current->parent;
      while (p != nullptr && current == p->left)
      {
        current = p;
        p = p->parent;
      }
      current = (p == nullptr ? fake : p);
    }
    return *this;
  }

  template<typename Key, typename Value>
  BSTIterator<Key, Value> BSTIterator<Key, Value>::operator--(int)
  {
    BSTIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template<typename Key, typename Value>
  bool BSTIterator<Key, Value>::operator==(const BSTIterator& other) const
  {
    return current == other.current;
  }

  template<typename Key, typename Value>
  bool BSTIterator<Key, Value>::operator!=(const BSTIterator& other) const
  {
    return !(*this == other);
  }

  template<typename Key, typename Value>
  typename BSTIterator<Key, Value>::NodePtr BSTIterator<Key, Value>::getNode() const
  {
    return current;
  }

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value>::BSTConstIterator(): current(nullptr), fake(nullptr)
  {}

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value>::BSTConstIterator(NodePtr ptr, NodePtr fk): current(ptr), fake(fk)
  {}

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value>::BSTConstIterator(const BSTIterator<Key, Value>& it)
    : current(it.getNode()), fake(it.fake)
  {}

  template<typename Key, typename Value>
  std::pair<const Key&, const Value&> BSTConstIterator<Key, Value>::operator*() const
  {
    return {current->key, current->value};
  }

  template<typename Key, typename Value>
  const Key& BSTConstIterator<Key, Value>::key() const
  {
    return current->key;
  }

  template<typename Key, typename Value>
  const Value& BSTConstIterator<Key, Value>::value() const
  {
    return current->value;
  }

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value>& BSTConstIterator<Key, Value>::operator++()
  {
    if (current == fake)
    {
      return *this;
    }
    if (current->right != fake)
    {
      current = current->right;
      while (current->left != fake)
      {
        current = current->left;
      }
    }
    else
    {
      NodePtr p = current->parent;
      while (p != nullptr && current == p->right)
      {
        current = p;
        p = p->parent;
      }
      current = (p == nullptr ? fake : p);
    }
    return *this;
  }

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value> BSTConstIterator<Key, Value>::operator++(int)
  {
    BSTConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template<typename Key, typename Value>
  bool BSTConstIterator<Key, Value>::operator==(const BSTConstIterator& other) const
  {
    return current == other.current;
  }

  template<typename Key, typename Value>
  bool BSTConstIterator<Key, Value>::operator!=(const BSTConstIterator& other) const
  {
    return !(*this == other);
  }

  template<typename Key, typename Value>
  typename BSTConstIterator<Key, Value>::NodePtr BSTConstIterator<Key, Value>::getNode() const
  {
    return current;
  }
}

#endif
