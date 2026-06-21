#ifndef BSTREE_HPP
#define BSTREE_HPP

#include "node.hpp"
#include "iterators.hpp"
#include <functional>
#include <stdexcept>

namespace velizade
{
  template<typename Key, typename Value, typename Compare = std::less<Key>>
  class BSTree
  {
  private:
    using NodePtr = Node<Key, Value>*;
    NodePtr root;
    NodePtr fake;
    Compare comp;

    NodePtr findNode(const Key& k) const
    {
      NodePtr cur = root;
      while (cur != fake)
      {
        if (comp(k, cur->key))
        {
          cur = cur->left;
        }
        else if (comp(cur->key, k))
        {
          cur = cur->right;
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
      if (node == fake || node == nullptr)
      {
        return;
      }
      clear(node->left);
      clear(node->right);
      delete node;
    }

    size_t heightRec(NodePtr node) const
    {
      if (node == fake || node == nullptr)
      {
        return 0;
      }
      size_t hLeft = heightRec(node->left);
      size_t hRight = heightRec(node->right);
      return 1 + (hLeft > hRight ? hLeft : hRight);
    }

    NodePtr rotateLeftImpl(NodePtr x)
    {
      if (x == fake || x->parent == nullptr)
      {
        return x;
      }
      NodePtr p = x->parent;
      if (p->right != x)
      {
        return x;
      }

      p->right = x->left;
      if (x->left != fake)
      {
        x->left->parent = p;
      }

      x->left = p;
      x->parent = p->parent;

      if (p->parent == nullptr)
      {
        root = x;
      }
      else if (p == p->parent->left)
      {
        p->parent->left = x;
      }
      else
      {
        p->parent->right = x;
      }

      p->parent = x;
      return x;
    }

    NodePtr rotateRightImpl(NodePtr x)
    {
      if (x == fake || x->parent == nullptr)
      {
        return x;
      }
      NodePtr p = x->parent;
      if (p->left != x)
      {
        return x;
      }

      p->left = x->right;
      if (x->right != fake)
      {
        x->right->parent = p;
      }

      x->right = p;
      x->parent = p->parent;

      if (p->parent == nullptr)
      {
        root = x;
      }
      else if (p == p->parent->left)
      {
        p->parent->left = x;
      }
      else
      {
        p->parent->right = x;
      }

      p->parent = x;
      return x;
    }

  public:
    using iterator = BSTIterator<Key, Value>;
    using const_iterator = BSTConstIterator<Key, Value>;

    BSTree()
    {
      fake = new Node();
      root = fake;
    }

    ~BSTree()
    {
      clear(root);
      delete fake;
    }

    void push(const Key& k, const Value& v)
    {
      if (root == fake)
      {
        NodePtr newNode = new Node(k, v);
        newNode->left = fake;
        newNode->right = fake;
        root = newNode;
        return;
      }

      NodePtr cur = root;
      NodePtr parent = nullptr;
      while (cur != fake)
      {
        parent = cur;
        if (comp(k, cur->key))
        {
          cur = cur->left;
        }
        else if (comp(cur->key, k))
        {
          cur = cur->right;
        }
        else
        {
          cur->value = v;
          return;
        }
      }

      NodePtr newNode = new Node(k, v);
      newNode->left = fake;
      newNode->right = fake;
      newNode->parent = parent;
      if (comp(k, parent->key))
      {
        parent->left = newNode;
      }
      else
      {
        parent->right = newNode;
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

      if (node->left == fake && node->right == fake)
      {
        NodePtr p = node->parent;
        if (p == nullptr)
        {
          root = fake;
        }
        else if (p->left == node)
        {
          p->left = fake;
        }
        else
        {
          p->right = fake;
        }
        delete node;
        return val;
      }

      if (node->left == fake || node->right == fake)
      {
        NodePtr child = (node->left != fake) ? node->left : node->right;
        NodePtr p = node->parent;
        child->parent = p;
        if (p == nullptr)
        {
          root = child;
        }
        else if (p->left == node)
        {
          p->left = child;
        }
        else
        {
          p->right = child;
        }
        delete node;
        return val;
      }

      NodePtr succ = node->right;
      while (succ->left != fake)
      {
        succ = succ->left;
      }

      Key succKey = succ->key;
      Value succVal = succ->value;

      NodePtr child = succ->right;
      NodePtr p = succ->parent;
      if (p->left == succ)
      {
        p->left = child;
      }
      else
      {
        p->right = child;
      }
      if (child != fake)
      {
        child->parent = p;
      }
      delete succ;

      node->key = succKey;
      node->value = succVal;
      return val;
    }

    size_t height() const
    {
      return heightRec(root);
    }

    size_t height(const_iterator it) const
    {
      NodePtr node = it.getNode();
      if (node == fake || node == nullptr)
      {
        return 0;
      }
      return heightRec(node);
    }

    iterator begin()
    {
      if (root == fake)
      {
        return iterator(fake, fake);
      }
      NodePtr cur = root;
      while (cur->left != fake)
      {
        cur = cur->left;
      }
      return iterator(cur, fake);
    }

    const_iterator begin() const
    {
      if (root == fake)
      {
        return const_iterator(fake, fake);
      }
      NodePtr cur = root;
      while (cur->left != fake)
      {
        cur = cur->left;
      }
      return const_iterator(cur, fake);
    }

    const_iterator cbegin() const
    {
      return begin();
    }

    iterator end()
    {
      return iterator(fake, fake);
    }

    const_iterator end() const
    {
      return const_iterator(fake, fake);
    }

    const_iterator cend() const
    {
      return end();
    }

    const_iterator rotateLeft(const_iterator it)
    {
      NodePtr x = it.getNode();
      if (x == fake || x->parent == nullptr)
      {
        return it;
      }
      NodePtr result = rotateLeftImpl(x);
      return const_iterator(result, fake);
    }

    const_iterator rotateRight(const_iterator it)
    {
      NodePtr x = it.getNode();
      if (x == fake || x->parent == nullptr)
      {
        return it;
      }
      NodePtr result = rotateRightImpl(x);
      return const_iterator(result, fake);
    }

    const_iterator rotateLargeLeft(const_iterator it)
    {
      NodePtr x = it.getNode();
      if (x == fake || x->parent == nullptr)
      {
        return it;
      }
      NodePtr y = x->left;
      if (y == fake)
      {
        return it;
      }
      NodePtr afterFirst = rotateRightImpl(y);
      NodePtr result = rotateLeftImpl(afterFirst);
      return const_iterator(result, fake);
    }

    const_iterator rotateLargeRight(const_iterator it)
    {
      NodePtr x = it.getNode();
      if (x == fake || x->parent == nullptr)
      {
        return it;
      }
      NodePtr y = x->right;
      if (y == fake)
      {
        return it;
      }
      NodePtr afterFirst = rotateLeftImpl(y);
      NodePtr result = rotateRightImpl(afterFirst);
      return const_iterator(result, fake);
    }

    bool contains(const Key& k) const
    {
      return findNode(k) != nullptr;
    }

    bool empty() const
    {
      return root == fake;
    }
  };
}

#endif
