#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "vector.hpp"
#include <algorithm>
#include <utility>

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
    void inorderCollect(AVLNode<K, T>* node, Vector<std::pair<K, T>>& out) const;
    void clearTree(AVLNode<K, T>* node);
    bool findNode(AVLNode<K, T>* node, const K& key, T& value) const;
    void copyTree(AVLNode<K, T>*& dest, const AVLNode<K, T>* src);
    AVLNode<K, T>* findNodePtr(AVLNode<K, T>* node, const K& key);
    AVLNode<K, T>* findNodePtr(AVLNode<K, T>* node, const K& key) const;

  public:
    AVLTree();
    ~AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    AVLTree& operator=(const AVLTree& other);
    AVLTree& operator=(AVLTree&& other) noexcept;

    bool insert(const K& key, const T& value);
    bool remove(const K& key);
    bool find(const K& key, T& value) const;
    bool find(const K& key) const;
    T* findPtr(const K& key);
    const T* findPtr(const K& key) const;
    Vector<std::pair<K, T>> getAll() const;
    void clear();
  };
}

template <typename K, typename T>
velizade::AVLNode<K, T>::AVLNode(const K& k, const T& v):
    key(k), value(v), left(nullptr), right(nullptr), height(1) {}

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
void velizade::AVLTree<K, T>::inorderCollect(AVLNode<K, T>* node, Vector<std::pair<K, T>>& out) const
{
  if (node)
  {
    inorderCollect(node->left, out);
    out.push_back({node->key, node->value});
    inorderCollect(node->right, out);
  }
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
  return (key < node->key) ? findNode(node->left, key, value) : findNode(node->right, key, value);
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
  copyTree(dest->left, src->left);
  copyTree(dest->right, src->right);
  updateHeight(dest);
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
  return (key < node->key) ? findNodePtr(node->left, key) : findNodePtr(node->right, key);
}

template <typename K, typename T>
velizade::AVLNode<K, T>* velizade::AVLTree<K, T>::findNodePtr(AVLNode<K, T>* node, const K& key) const
{
  if (!node)
  {
    return nullptr;
  }
  if (key == node->key)
  {
    return node;
  }
  return (key < node->key) ? findNodePtr(node->left, key) : findNodePtr(node->right, key);
}

template <typename K, typename T>
velizade::AVLTree<K, T>::AVLTree() : root(nullptr) {}

template <typename K, typename T>
velizade::AVLTree<K, T>::~AVLTree()
{
  clearTree(root);
}

template <typename K, typename T>
velizade::AVLTree<K, T>::AVLTree(const AVLTree& other) : root(nullptr)
{
  copyTree(root, other.root);
}

template <typename K, typename T>
velizade::AVLTree<K, T>::AVLTree(AVLTree&& other) noexcept : root(other.root)
{
  other.root = nullptr;
}

template <typename K, typename T>
velizade::AVLTree<K, T>& velizade::AVLTree<K, T>::operator=(const AVLTree& other)
{
  if (this == &other)
  {
    return *this;
  }
  clearTree(root);
  root = nullptr;
  copyTree(root, other.root);
  return *this;
}

template <typename K, typename T>
velizade::AVLTree<K, T>& velizade::AVLTree<K, T>::operator=(AVLTree&& other) noexcept
{
  if (this == &other)
  {
    return *this;
  }
  clearTree(root);
  root = other.root;
  other.root = nullptr;
  return *this;
}

template <typename K, typename T>
bool velizade::AVLTree<K, T>::insert(const K& key, const T& value)
{
  if (find(key))
  {
    return false;
  }
  root = insertNode(root, key, value);
  return true;
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
  AVLNode<K, T>* node = findNodePtr(root, key);
  return node ? &(node->value) : nullptr;
}

template <typename K, typename T>
velizade::Vector<std::pair<K, T>> velizade::AVLTree<K, T>::getAll() const
{
  velizade::Vector<std::pair<K, T>> res;
  inorderCollect(root, res);
  return res;
}

template <typename K, typename T>
void velizade::AVLTree<K, T>::clear()
{
  clearTree(root);
  root = nullptr;
}

#endif
