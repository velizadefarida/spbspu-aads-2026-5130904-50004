#ifndef BSTREE_HPP
#define BSTREE_HPP

#include "node.hpp"
#include "iterators.hpp"
#include <functional>
#include <stdexcept>
#include <algorithm>

namespace velizade
{
  template<typename Key, typename Value, typename Compare = std::less<Key>>
  class BSTree
  {
  private:
    using NodePtr = Node<Key, Value>*;
    NodePtr root_;
    Compare comp_;

    NodePtr findNode(const Key& k) const
    {
      NodePtr cur = root_;
      while (cur != nullptr && !cur->isFake())
      {
        if (comp_(k, cur->key))
        {
          cur = cur->left_;
        }
        else if (comp_(cur->key, k))
        {
          cur = cur->right_;
        }
        else
        {
          return cur;
        }
      }
      return nullptr;
    }

    void clear(NodePtr node)
    {
      if (node == nullptr || node->isFake())
      {
        return;
      }
      clear(node->left_);
      clear(node->right_);
      delete node;
    }

    NodePtr copyNode(NodePtr node, NodePtr parent)
    {
      if (node == nullptr || node->isFake())
      {
        return std::addressof(Node<Key, Value>::fakeLeaf_);
      }
      NodePtr newNode = new Node<Key, Value>(node->key, node->value, parent);
      newNode->left_ = copyNode(node->left_, newNode);
      newNode->right_ = copyNode(node->right_, newNode);
      return newNode;
    }

    size_t heightRec(NodePtr node) const
    {
      if (node == nullptr || node->isFake())
      {
        return 0;
      }
      return 1 + std::max(heightRec(node->left_), heightRec(node->right_));
    }

    NodePtr rotateLeftImpl(NodePtr x)
    {
      if (x == nullptr || x->isFake() || x->parent_ == nullptr)
      {
        return x;
      }
      NodePtr p = x->parent_;
      if (p->right_ != x)
      {
        return x;
      }

      NodePtr xLeft = x->left_;
      p->right_ = xLeft;
      if (xLeft != nullptr && !xLeft->isFake())
      {
        xLeft->parent_ = p;
      }

      x->left_ = p;
      x->parent_ = p->parent_;

      if (p->parent_ == nullptr)
      {
        root_ = x;
      }
      else if (p->parent_->left_ == p)
      {
        p->parent_->left_ = x;
      }
      else
      {
        p->parent_->right_ = x;
      }

      p->parent_ = x;
      return x;
    }

    NodePtr rotateRightImpl(NodePtr x)
    {
      if (x == nullptr || x->isFake() || x->parent_ == nullptr)
      {
        return x;
      }
      NodePtr p = x->parent_;
      if (p->left_ != x)
      {
        return x;
      }

      NodePtr xRight = x->right_;
      p->left_ = xRight;
      if (xRight != nullptr && !xRight->isFake())
      {
        xRight->parent_ = p;
      }

      x->right_ = p;
      x->parent_ = p->parent_;

      if (p->parent_ == nullptr)
      {
        root_ = x;
      }
      else if (p->parent_->left_ == p)
      {
        p->parent_->left_ = x;
      }
      else
      {
        p->parent_->right_ = x;
      }

      p->parent_ = x;
      return x;
    }

  public:
    using iterator = BSTIterator<Key, Value>;
    using const_iterator = BSTConstIterator<Key, Value>;

    BSTree() : root_(std::addressof(Node<Key, Value>::fakeLeaf_)) {}

    BSTree(const BSTree& other) : root_(std::addressof(Node<Key, Value>::fakeLeaf_))
    {
      root_ = copyNode(other.root_, nullptr);
    }

    BSTree(BSTree&& other) noexcept
      : root_(std::addressof(Node<Key, Value>::fakeLeaf_))
    {
      std::swap(root_, other.root_);
    }

    ~BSTree()
    {
      clear(root_);
    }

    BSTree& operator=(const BSTree& other)
    {
      if (this == &other)
      {
        return *this;
      }
      clear(root_);
      root_ = copyNode(other.root_, nullptr);
      return *this;
    }

    BSTree& operator=(BSTree&& other) noexcept
    {
      if (this == &other)
      {
        return *this;
      }
      clear(root_);
      root_ = other.root_;
      other.root_ = std::addressof(Node<Key, Value>::fakeLeaf_);
      return *this;
    }

    void push(const Key& k, const Value& v)
    {
      if (root_->isFake())
      {
        root_ = new Node<Key, Value>(k, v, nullptr);
        return;
      }

      NodePtr cur = root_;
      NodePtr parent = nullptr;
      while (!cur->isFake())
      {
        parent = cur;
        if (comp_(k, cur->key))
        {
          cur = cur->left_;
        }
        else if (comp_(cur->key, k))
        {
          cur = cur->right_;
        }
        else
        {
          cur->value = v;
          return;
        }
      }

      NodePtr newNode = new Node<Key, Value>(k, v, parent);
      if (comp_(k, parent->key))
      {
        parent->left_ = newNode;
      }
      else
      {
        parent->right_ = newNode;
      }
    }

    Value get(const Key& k) const
    {
      NodePtr node = findNode(k);
      if (node == nullptr)
      {
        throw std::out_of_range("Key not found");
      }
      return node->value;
    }

    Value drop(const Key& k)
    {
      NodePtr node = findNode(k);
      if (node == nullptr)
      {
        throw std::out_of_range("Key not found");
      }

      Value val = node->value;


      if (node->left_->isFake() && node->right_->isFake())
      {
        NodePtr p = node->parent_;
        if (p == nullptr)
        {
          root_ = std::addressof(Node<Key, Value>::fakeLeaf_);
        }
        else if (p->left_ == node)
        {
          p->left_ = std::addressof(Node<Key, Value>::fakeLeaf_);
        }
        else
        {
          p->right_ = std::addressof(Node<Key, Value>::fakeLeaf_);
        }
        delete node;
        return val;
      }

      if (node->left_->isFake() || node->right_->isFake())
      {
        NodePtr child = node->left_->isFake() ? node->right_ : node->left_;
        NodePtr p = node->parent_;
        child->parent_ = p;
        if (p == nullptr)
        {
          root_ = child;
        }
        else if (p->left_ == node)
        {
          p->left_ = child;
        }
        else
        {
          p->right_ = child;
        }
        delete node;
        return val;
      }

      NodePtr succ = node->right_;
      while (!succ->left_->isFake())
      {
        succ = succ->left_;
      }

      Key succKey = succ->key;
      Value succVal = succ->value;

      NodePtr child = succ->right_;
      NodePtr p = succ->parent_;
      if (p->left_ == succ)
      {
        p->left_ = child;
      }
      else
      {
        p->right_ = child;
      }
      if (!child->isFake())
      {
        child->parent_ = p;
      }
      delete succ;

      node->key = succKey;
      node->value = succVal;
      return val;
    }

    size_t height() const
    {
      return heightRec(root_);
    }

    size_t height(const_iterator it) const
    {
      NodePtr node = it.getNode();
      if (node == nullptr || node->isFake())
      {
        return 0;
      }
      return heightRec(node);
    }

    iterator begin()
    {
      if (root_->isFake())
      {
        return iterator(std::addressof(Node<Key, Value>::fakeLeaf_));
      }
      NodePtr cur = root_;
      while (!cur->left_->isFake())
      {
        cur = cur->left_;
      }
      return iterator(cur);
    }

    const_iterator begin() const
    {
      if (root_->isFake())
      {
        return const_iterator(std::addressof(Node<Key, Value>::fakeLeaf_));
      }
      NodePtr cur = root_;
      while (!cur->left_->isFake())
      {
        cur = cur->left_;
      }
      return const_iterator(cur);
    }

    const_iterator cbegin() const
    {
      return begin();
    }

    iterator end()
    {
      return iterator(std::addressof(Node<Key, Value>::fakeLeaf_));
    }

    const_iterator end() const
    {
      return const_iterator(std::addressof(Node<Key, Value>::fakeLeaf_));
    }

    const_iterator cend() const
    {
      return end();
    }

    const_iterator rotateLeft(const_iterator it)
    {
      NodePtr x = it.getNode();
      if (x == nullptr || x->isFake() || x->parent_ == nullptr)
      {
        return it;
      }
      NodePtr result = rotateLeftImpl(x);
      return const_iterator(result);
    }

    const_iterator rotateRight(const_iterator it)
    {
      NodePtr x = it.getNode();
      if (x == nullptr || x->isFake() || x->parent_ == nullptr)
      {
        return it;
      }
      NodePtr result = rotateRightImpl(x);
      return const_iterator(result);
    }

    const_iterator rotateLargeLeft(const_iterator it)
    {
      NodePtr x = it.getNode();
      if (x == nullptr || x->isFake() || x->parent_ == nullptr)
      {
        return it;
      }

      NodePtr p = x->parent_;
      if (p == nullptr)
      {
        return it;
      }
      NodePtr g = p->parent_;
      if (g == nullptr)
      {
        return it;
      }

      if (p->right_ != x || g->left_ != p)
      {
        return it;
      }

      rotateLeftImpl(p);
      rotateRightImpl(g);

      return const_iterator(x);
    }

    const_iterator rotateLargeRight(const_iterator it)
    {
      NodePtr x = it.getNode();
      if (x == nullptr || x->isFake() || x->parent_ == nullptr)
      {
        return it;
      }

      NodePtr p = x->parent_;
      if (p == nullptr)
      {
        return it;
      }
      NodePtr g = p->parent_;
      if (g == nullptr)
      {
        return it;
      }

      if (p->left_ != x || g->right_ != p)
      {
        return it;
      }

      rotateRightImpl(p);
      rotateLeftImpl(g);

      return const_iterator(x);
    }

    bool contains(const Key& k) const
    {
      return findNode(k) != nullptr;
    }

    bool empty() const
    {
      return root_->isFake();
    }
  };
}

#endif
