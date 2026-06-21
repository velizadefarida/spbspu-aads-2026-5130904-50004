#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace velizade
{
  template<typename Key, typename Value>
  struct Node
  {
    Node();
    Node(const Key& k, const Value& v, Node* parent = nullptr);

    std::pair<const Key, Value> data_;
    Node* left_;
    Node* right_;
    Node* parent_;

    static Node fakeLeaf_;

    bool isFake() const noexcept;
  };
}

template<typename Key, typename Value>
velizade::Node<Key, Value>::Node():
    data_(),
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr)
{
  left_ = right_ = this;
}

template<typename Key, typename Value>
velizade::Node<Key, Value>::Node(const Key& k, const Value& v, Node* parent):
    data_(k, v),
    left_(std::addressof(fakeLeaf_)),
    right_(std::addressof(fakeLeaf_)),
    parent_(parent)
{}

template<typename Key, typename Value>
velizade::Node<Key, Value> velizade::Node<Key, Value>::fakeLeaf_;

template<typename Key, typename Value>
bool velizade::Node<Key, Value>::isFake() const noexcept
{
  return this == std::addressof(fakeLeaf_);
}

#endif
