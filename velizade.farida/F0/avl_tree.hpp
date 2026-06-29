#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "vector.hpp"
#include <algorithm>
#include <utility>
#include <stack>
#include <stdexcept>

namespace velizade
{
  template <typename K, typename T>
  struct AVLNode
  {
    K key;
    T value;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(const K& k, const T& v);
  };

  template <typename K, typename T>
  class AVLTree
  {
  private:
    AVLNode<K, T>* root;

    int height(AVLNode<K, T>* node) const;
    int balanceFactor(AVLNode<K, T>* node) const;
    void updateHeight(AVLNode<K, T>* node);
    AVLNode<K, T>* rotateRight(AVLNode<K, T>* y);
    AVLNode<K, T>* rotateLeft(AVLNode<K, T>* x);
    AVLNode<K, T>* balance(AVLNode<K, T>* node);
    AVLNode<K, T>* insertNode(AVLNode<K, T>* node, const K& key, const T& value);
    AVLNode<K, T>* findMin(AVLNode<K, T>* node);
    AVLNode<K, T>* removeNode(AVLNode<K, T>* node, const K& key);
    bool findNode(AVLNode<K, T>* node, const K& key, T& value) const;
    void clearTree(AVLNode<K, T>* node);
    void copyTree(AVLNode<K, T>*& dest, const AVLNode<K, T>* src);
    AVLNode<K, T>* findNodePtr(AVLNode<K, T>* node, const K& key);
    const AVLNode<K, T>* findNodePtrConst(const AVLNode<K, T>* node, const K& key) const;

  public:
    class Iterator
    {
    private:
      std::stack<AVLNode<K, T>*> stack;
      AVLNode<K, T>* current;
      void pushLeftBranch(AVLNode<K, T>* node);
    public:
      Iterator();
      explicit Iterator(AVLNode<K, T>* root);
      Iterator& operator++();
      bool operator!=(const Iterator& other) const;
      bool operator==(const Iterator& other) const;
      std::pair<const K, T&> operator*() const;
      std::pair<const K, T&>* operator->();
      K& key();
      T& value();
    };

    class ConstIterator
    {
    private:
      std::stack<const AVLNode<K, T>*> stack;
      const AVLNode<K, T>* current;
      void pushLeftBranch(const AVLNode<K, T>* node);
    public:
      ConstIterator();
      explicit ConstIterator(const AVLNode<K, T>* root);
      ConstIterator& operator++();
      bool operator!=(const ConstIterator& other) const;
      bool operator==(const ConstIterator& other) const;
      std::pair<const K, const T&> operator*() const;
      const std::pair<const K, const T&>* operator->() const;
    };

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    AVLTree();
    ~AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    AVLTree& operator=(const AVLTree& other);
    AVLTree& operator=(AVLTree&& other) noexcept;
    void swap(AVLTree& other) noexcept;

    std::pair<iterator, bool> insert(const K& key, const T& value);
    bool remove(const K& key);
    bool find(const K& key, T& value) const;
    bool find(const K& key) const;
    T* findPtr(const K& key);
    const T* findPtr(const K& key) const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void clear();
  };
}

template <typename K, typename T>
velizade::AVLNode<K, T>::AVLNode(const K& k, const T& v)
  : key(k)
  , value(v)
  , left(nullptr)
  , right(nullptr)
  , height(1)
{}

template <typename K, typename T>
int velizade::AVLTree<K, T>::height(AVLNode<K, T>* node) const
{
  return node ? node->height : 0;
}

template <typename K, typename T>
int velizade::AVLTree<K, T>::balanceFactor(AVLNode<K, T>* node) const
{
  return node ? height(node->left) - height(node->right) : 0;
}

template <typename K, typename T>
void velizade::AVLTree<K, T>::updateHeight(AVLNode<K, T>* node)
{
  if (node)
  {
    node->height = 1 + std::max(height(node->left), height(node->right));
  }
}

template <typename K, typename T>
velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::rotateRight(AVLNode<K, T>* y)
{
  AVLNode<K, T>* x = y->left;
  AVLNode<K, T>* T2 = x->right;
  x->right = y;
  y->left = T2;
  updateHeight(y);
  updateHeight(x);
  return x;
}

template <typename K, typename T>
velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::rotateLeft(AVLNode<K, T>* x)
{
  AVLNode<K, T>* y = x->right;
  AVLNode<K, T>* T2 = y->left;
  y->left = x;
  x->right = T2;
  updateHeight(x);
  updateHeight(y);
  return y;
}

template <typename K, typename T>
velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::balance(AVLNode<K, T>* node)
{
  if (!node)
  {
    return nullptr;
  }
  updateHeight(node);
  int bf = balanceFactor(node);
  if (bf > 1)
  {
    if (balanceFactor(node->left) < 0)
    {
      node->left = rotateLeft(node->left);
    }
    return rotateRight(node);
  }
  if (bf < -1)
  {
    if (balanceFactor(node->right) > 0)
    {
      node->right = rotateRight(node->right);
    }
    return rotateLeft(node);
  }
  return node;
}

template <typename K, typename T>
velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::insertNode(AVLNode<K, T>* node, const K& key, const T& value)
{
  if (!node)
  {
    return new AVLNode<K, T>(key, value);
  }
  if (key < node->key)
  {
    node->left = insertNode(node->left, key, value);
  }
  else if (key > node->key)
  {
    node->right = insertNode(node->right, key, value);
  }
  else
  {
    return node;
  }
  return balance(node);
}

template <typename K, typename T>
velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::findMin(AVLNode<K, T>* node)
{
  while (node->left)
  {
    node = node->left;
  }
  return node;
}

template <typename K, typename T>
velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::removeNode(AVLNode<K, T>* node, const K& key)
{
  if (!node)
  {
    return nullptr;
  }
  if (key < node->key)
  {
    node->left = removeNode(node->left, key);
  }
  else if (key > node->key)
  {
    node->right = removeNode(node->right, key);
  }
  else
  {
    if (!node->left || !node->right)
    {
      AVLNode<K, T>* temp = node->left ? node->left : node->right;
      delete node;
      return temp;
    }
    else
    {
      AVLNode<K, T>* temp = findMin(node->right);
      node->key = temp->key;
      node->value = temp->value;
      node->right = removeNode(node->right, temp->key);
    }
  }
  return balance(node);
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::findNode(AVLNode<K, T>* node, const K& key, T& value) const
{
  if (!node)
  {
    return false;
  }
  if (key == node->key)
  {
    value = node->value;
    return true;
  }
  return (key < node->key) ? findNode(node->left, key, value)
                           : findNode(node->right, key, value);
}

template <typename K, typename T>
void velizade::AVLTree<K, T>::clearTree(AVLNode<K, T>* node)
{
  if (node)
  {
    clearTree(node->left);
    clearTree(node->right);
    delete node;
  }
}

template <typename K, typename T>
void velizade::AVLTree<K, T>::copyTree(AVLNode<K, T>*& dest, const AVLNode<K, T>* src)
{
  if (!src)
  {
    dest = nullptr;
    return;
  }
  dest = new AVLNode<K, T>(src->key, src->value);
  try
  {
    copyTree(dest->left, src->left);
    copyTree(dest->right, src->right);
    updateHeight(dest);
  }
  catch (...)
  {
    clearTree(dest);
    dest = nullptr;
    throw;
  }
}

template <typename K, typename T>
velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::findNodePtr(AVLNode<K, T>* node, const K& key)
{
  if (!node)
  {
    return nullptr;
  }
  if (key == node->key)
  {
    return node;
  }
  return (key < node->key) ? findNodePtr(node->left, key)
                           : findNodePtr(node->right, key);
}

template <typename K, typename T>
const velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::findNodePtrConst(const AVLNode<K, T>* node, const K& key) const
{
  if (!node)
  {
    return nullptr;
  }
  if (key == node->key)
  {
    return node;
  }
  return (key < node->key) ? findNodePtrConst(node->left, key)
                           : findNodePtrConst(node->right, key);
}

template <typename K, typename T>
void velizade::AVLTree<K, T>::Iterator::pushLeftBranch(AVLNode<K, T>* node)
{
  while (node)
  {
    stack.push(node);
    node = node->left;
  }
}

template <typename K, typename T>
velizade::AVLTree<K, T>::Iterator::Iterator()
  : current(nullptr)
{}

template <typename K, typename T>
velizade::AVLTree<K, T>::Iterator::Iterator(AVLNode<K, T>* root)
  : current(nullptr)
{
  if (root)
  {
    pushLeftBranch(root);
    if (!stack.empty())
    {
      current = stack.top();
    }
  }
}

template <typename K, typename T>
typename velizade::AVLTree<K, T>::Iterator& velizade::AVLTree<K, T>::Iterator::operator++()
{
  if (!stack.empty())
  {
    AVLNode<K, T>* node = stack.top();
    stack.pop();
    if (node->right)
    {
      pushLeftBranch(node->right);
    }
    if (!stack.empty())
    {
      current = stack.top();
    }
    else
    {
      current = nullptr;
    }
  }
  else
  {
    current = nullptr;
  }
  return *this;
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::Iterator::operator!=(const Iterator& other) const
{
  return current != other.current;
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::Iterator::operator==(const Iterator& other) const
{
  return current == other.current;
}

template <typename K, typename T>
std::pair<const K, T&> velizade::AVLTree<K, T>::Iterator::operator*() const
{
  return std::pair<const K, T&>(current->key, current->value);
}

template <typename K, typename T>
std::pair<const K, T&>* velizade::AVLTree<K, T>::Iterator::operator->()
{
  return &(operator*());
}

template <typename K, typename T>
K& velizade::AVLTree<K, T>::Iterator::key()
{
  return current->key;
}

template <typename K, typename T>
T& velizade::AVLTree<K, T>::Iterator::value()
{
  return current->value;
}

template <typename K, typename T>
void velizade::AVLTree<K, T>::ConstIterator::pushLeftBranch(const AVLNode<K, T>* node)
{
  while (node)
  {
    stack.push(node);
    node = node->left;
  }
}

template <typename K, typename T>
velizade::AVLTree<K, T>::ConstIterator::ConstIterator()
  : current(nullptr)
{}

template <typename K, typename T>
velizade::AVLTree<K, T>::ConstIterator::ConstIterator(const AVLNode<K, T>* root)
  : current(nullptr)
{
  if (root)
  {
    pushLeftBranch(root);
    if (!stack.empty())
    {
      current = stack.top();
    }
  }
}

template <typename K, typename T>
typename velizade::AVLTree<K, T>::ConstIterator& velizade::AVLTree<K, T>::ConstIterator::operator++()
{
  if (!stack.empty())
  {
    const AVLNode<K, T>* node = stack.top();
    stack.pop();
    if (node->right)
    {
      pushLeftBranch(node->right);
    }
    if (!stack.empty())
    {
      current = stack.top();
    }
    else
    {
      current = nullptr;
    }
  }
  else
  {
    current = nullptr;
  }
  return *this;
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::ConstIterator::operator!=(const ConstIterator& other) const
{
  return current != other.current;
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::ConstIterator::operator==(const ConstIterator& other) const
{
  return current == other.current;
}

template <typename K, typename T>
std::pair<const K, const T&> velizade::AVLTree<K, T>::ConstIterator::operator*() const
{
  return std::pair<const K, const T&>(current->key, current->value);
}

template <typename K, typename T>
const std::pair<const K, const T&>* velizade::AVLTree<K, T>::ConstIterator::operator->() const
{
  return &(operator*());
}

template <typename K, typename T>
velizade::AVLTree<K, T>::AVLTree()
  : root(nullptr)
{}

template <typename K, typename T>
velizade::AVLTree<K, T>::~AVLTree()
{
  clearTree(root);
}

template <typename K, typename T>
velizade::AVLTree<K, T>::AVLTree(const AVLTree& other)
  : root(nullptr)
{
  copyTree(root, other.root);
}

template <typename K, typename T>
velizade::AVLTree<K, T>::AVLTree(AVLTree&& other) noexcept
  : root(other.root)
{
  other.root = nullptr;
}

template <typename K, typename T>
velizade::AVLTree<K, T>& velizade::AVLTree<K, T>::operator=(const AVLTree& other)
{
  if (this != &other)
  {
    AVLTree copy(other);
    swap(copy);
  }
  return *this;
}

template <typename K, typename T>
velizade::AVLTree<K, T>& velizade::AVLTree<K, T>::operator=(AVLTree&& other) noexcept
{
  if (this != &other)
  {
    clearTree(root);
    root = other.root;
    other.root = nullptr;
  }
  return *this;
}

template <typename K, typename T>
void velizade::AVLTree<K, T>::swap(AVLTree& other) noexcept
{
  std::swap(root, other.root);
}

template <typename K, typename T>
std::pair<typename velizade::AVLTree<K, T>::iterator, bool> velizade::AVLTree<K, T>::insert(const K& key, const T& value)
{
  if (find(key))
  {
    return std::make_pair(iterator(), false);
  }
  root = insertNode(root, key, value);
  return std::make_pair(iterator(root), true);
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::remove(const K& key)
{
  if (!find(key))
  {
    return false;
  }
  root = removeNode(root, key);
  return true;
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::find(const K& key, T& value) const
{
  return findNode(root, key, value);
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::find(const K& key) const
{
  T dummy;
  return find(key, dummy);
}

template <typename K, typename T>
T* velizade::AVLTree<K, T>::findPtr(const K& key)
{
  AVLNode<K, T>* node = findNodePtr(root, key);
  return node ? &(node->value) : nullptr;
}

template <typename K, typename T>
const T* velizade::AVLTree<K, T>::findPtr(const K& key) const
{
  const AVLNode<K, T>* node = findNodePtrConst(root, key);
  return node ? &(node->value) : nullptr;
}

template <typename K, typename T>
typename velizade::AVLTree<K, T>::iterator velizade::AVLTree<K, T>::begin()
{
  return iterator(root);
}

template <typename K, typename T>
typename velizade::AVLTree<K, T>::iterator velizade::AVLTree<K, T>::end()
{
  return iterator();
}

template <typename K, typename T>
typename velizade::AVLTree<K, T>::const_iterator velizade::AVLTree<K, T>::begin() const
{
  return const_iterator(root);
}

template <typename K, typename T>
typename velizade::AVLTree<K, T>::const_iterator velizade::AVLTree<K, T>::end() const
{
  return const_iterator();
}

template <typename K, typename T>
void velizade::AVLTree<K, T>::clear()
{
  clearTree(root);
  root = nullptr;
}

#endif
