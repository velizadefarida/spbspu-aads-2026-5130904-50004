#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include "node.hpp"
#include <iterator>
#include <utility>

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
    NodePtr node_;

    BSTIterator(NodePtr node) noexcept;

  public:
    using value_type = std::pair<const Key, Value>;
    using reference = value_type&;

    BSTIterator() = default;

    reference operator*() const noexcept;
    value_type* operator->() const noexcept;

    BSTIterator& operator++() noexcept;
    BSTIterator operator++(int) noexcept;
    BSTIterator& operator--() noexcept;
    BSTIterator operator--(int) noexcept;

    bool operator==(const BSTIterator& other) const noexcept;
    bool operator!=(const BSTIterator& other) const noexcept;

    NodePtr getNode() const noexcept;
  };

  template<typename Key, typename Value>
  class BSTConstIterator
  {
    using NodePtr = Node<Key, Value>*;
    NodePtr node_;

    BSTConstIterator(NodePtr node) noexcept;

  public:
    using value_type = const std::pair<const Key, Value>;
    using reference = value_type&;

    BSTConstIterator() = default;
    BSTConstIterator(const BSTIterator<Key, Value>& it) noexcept;

    reference operator*() const noexcept;
    value_type* operator->() const noexcept;

    BSTConstIterator& operator++() noexcept;
    BSTConstIterator operator++(int) noexcept;
    BSTConstIterator& operator--() noexcept;
    BSTConstIterator operator--(int) noexcept;

    bool operator==(const BSTConstIterator& other) const noexcept;
    bool operator!=(const BSTConstIterator& other) const noexcept;

    NodePtr getNode() const noexcept;
  };

  template<typename Key, typename Value>
  BSTIterator<Key, Value>::BSTIterator(NodePtr node) noexcept:
      node_(node ? node : std::addressof(Node<Key, Value>::fakeLeaf_))
  {}

  template<typename Key, typename Value>
  typename BSTIterator<Key, Value>::reference BSTIterator<Key, Value>::operator*() const noexcept
  {
    return node_->data_;
  }

  template<typename Key, typename Value>
  typename BSTIterator<Key, Value>::value_type* BSTIterator<Key, Value>::operator->() const noexcept
  {
    return std::addressof(node_->data_);
  }

  template<typename Key, typename Value>
  BSTIterator<Key, Value>& BSTIterator<Key, Value>::operator++() noexcept
  {
    if (node_->right_ != std::addressof(Node<Key, Value>::fakeLeaf_))
    {
      node_ = node_->right_;
      while (node_->left_ != std::addressof(Node<Key, Value>::fakeLeaf_))
      {
        node_ = node_->left_;
      }
    }
    else
    {
      NodePtr p = node_->parent_;
      while (p != nullptr && node_ == p->right_)
      {
        node_ = p;
        p = p->parent_;
      }
      node_ = (p == nullptr ? std::addressof(Node<Key, Value>::fakeLeaf_) : p);
    }
    return *this;
  }

  template<typename Key, typename Value>
  BSTIterator<Key, Value> BSTIterator<Key, Value>::operator++(int) noexcept
  {
    BSTIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template<typename Key, typename Value>
  BSTIterator<Key, Value>& BSTIterator<Key, Value>::operator--() noexcept
  {
    if (node_->isFake())
    {
      return *this;
    }
    if (node_->left_ != std::addressof(Node<Key, Value>::fakeLeaf_))
    {
      node_ = node_->left_;
      while (node_->right_ != std::addressof(Node<Key, Value>::fakeLeaf_))
      {
        node_ = node_->right_;
      }
    }
    else
    {
      NodePtr p = node_->parent_;
      while (p != nullptr && node_ == p->left_)
      {
        node_ = p;
        p = p->parent_;
      }
      node_ = (p == nullptr ? std::addressof(Node<Key, Value>::fakeLeaf_) : p);
    }
    return *this;
  }

  template<typename Key, typename Value>
  BSTIterator<Key, Value> BSTIterator<Key, Value>::operator--(int) noexcept
  {
    BSTIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template<typename Key, typename Value>
  bool BSTIterator<Key, Value>::operator==(const BSTIterator& other) const noexcept
  {
    return node_ == other.node_;
  }

  template<typename Key, typename Value>
  bool BSTIterator<Key, Value>::operator!=(const BSTIterator& other) const noexcept
  {
    return !(*this == other);
  }

  template<typename Key, typename Value>
  typename BSTIterator<Key, Value>::NodePtr BSTIterator<Key, Value>::getNode() const noexcept
  {
    return node_;
  }

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value>::BSTConstIterator(NodePtr node) noexcept:
      node_(node ? node : std::addressof(Node<Key, Value>::fakeLeaf_))
  {}

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value>::BSTConstIterator(const BSTIterator<Key, Value>& it) noexcept:
      node_(it.getNode())
  {}

  template<typename Key, typename Value>
  typename BSTConstIterator<Key, Value>::reference BSTConstIterator<Key, Value>::operator*() const noexcept
  {
    return node_->data_;
  }

  template<typename Key, typename Value>
  typename BSTConstIterator<Key, Value>::value_type* BSTConstIterator<Key, Value>::operator->() const noexcept
  {
    return std::addressof(node_->data_);
  }

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value>& BSTConstIterator<Key, Value>::operator++() noexcept
  {
    if (node_->right_ != std::addressof(Node<Key, Value>::fakeLeaf_))
    {
      node_ = node_->right_;
      while (node_->left_ != std::addressof(Node<Key, Value>::fakeLeaf_))
      {
        node_ = node_->left_;
      }
    }
    else
    {
      NodePtr p = node_->parent_;
      while (p != nullptr && node_ == p->right_)
      {
        node_ = p;
        p = p->parent_;
      }
      node_ = (p == nullptr ? std::addressof(Node<Key, Value>::fakeLeaf_) : p);
    }
    return *this;
  }

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value> BSTConstIterator<Key, Value>::operator++(int) noexcept
  {
    BSTConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value>& BSTConstIterator<Key, Value>::operator--() noexcept
  {
    if (node_->isFake())
    {
      return *this;
    }
    if (node_->left_ != std::addressof(Node<Key, Value>::fakeLeaf_))
    {
      node_ = node_->left_;
      while (node_->right_ != std::addressof(Node<Key, Value>::fakeLeaf_))
      {
        node_ = node_->right_;
      }
    }
    else
    {
      NodePtr p = node_->parent_;
      while (p != nullptr && node_ == p->left_)
      {
        node_ = p;
        p = p->parent_;
      }
      node_ = (p == nullptr ? std::addressof(Node<Key, Value>::fakeLeaf_) : p);
    }
    return *this;
  }

  template<typename Key, typename Value>
  BSTConstIterator<Key, Value> BSTConstIterator<Key, Value>::operator--(int) noexcept
  {
    BSTConstIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template<typename Key, typename Value>
  bool BSTConstIterator<Key, Value>::operator==(const BSTConstIterator& other) const noexcept
  {
    return node_ == other.node_;
  }

  template<typename Key, typename Value>
  bool BSTConstIterator<Key, Value>::operator!=(const BSTConstIterator& other) const noexcept
  {
    return !(*this == other);
  }

  template<typename Key, typename Value>
  typename BSTConstIterator<Key, Value>::NodePtr BSTConstIterator<Key, Value>::getNode() const noexcept
  {
    return node_;
  }
}

#endif
