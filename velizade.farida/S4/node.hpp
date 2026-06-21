#ifndef NODE_HPP
#define NODE_HPP

namespace velizade
{
  template<typename Key, typename Value>
  struct Node
  {
    Key key;
    Value value;
    Node* left;
    Node* right;
    Node* parent;
    bool isFake;

    Node() : isFake(true), left(nullptr), right(nullptr), parent(nullptr) {}

    Node(const Key& k, const Value& v):
        key(k), value(v), left(nullptr), right(nullptr), parent(nullptr), isFake(false) {}
  };
}

#endif
