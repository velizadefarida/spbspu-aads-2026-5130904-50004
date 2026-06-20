#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <functional>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hash_append.hpp>
#include "vector.hpp"
#include "hash_table_iterator.hpp"

namespace velizade
{
  struct SipHash
  {
    template<class T>
    std::size_t operator()(const T& key) const noexcept
    {
      boost::hash2::siphash_64 hasher;
      boost::hash2::hash_append(hasher, {}, key);
      return hasher.result();
    }
  };

  template<class Key, class Value, class Hash = SipHash, class Equal = std::equal_to<Key>>
  class HashTable
  {
  public:
    struct Cell
    {
      Key key;
      Value value;
      bool occupied = false;
    };

    using key_type = Key;
    using value_type = Value;
    using hasher = Hash;
    using key_equal = Equal;
    using iterator = HashTableIterator<Key, Value, Hash, Equal>;
    using const_iterator = HashTableIterator<Key, Value, Hash, Equal>;

    friend class HashTableIterator<Key, Value, Hash, Equal>;

    explicit HashTable(size_t numBuckets = 16, size_t bucketSize = 4);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    ~HashTable();

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    void swap(HashTable& other) noexcept;

    void add(const Key& k, const Value& v);
    Value drop(const Key& k);
    bool has(const Key& k) const;
    void rehash(size_t newNumBuckets);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    Cell* find(const Key& k);
    const Cell* find(const Key& k) const;

  private:
    size_t numBuckets_;
    size_t bucketSize_;
    Cell* table_;
    Vector<Cell> overflow_;
    Hash hash_;
    Equal equal_;

    size_t getBucketIndex(const Key& k) const;
    Cell* findInBucket(size_t idx, const Key& k);
    const Cell* findInBucket(size_t idx, const Key& k) const;
    Cell* findInOverflow(const Key& k);
    const Cell* findInOverflow(const Key& k) const;
  };
}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTable<Key, Value, Hash, Equal>::HashTable(size_t numBuckets, size_t bucketSize) :
  numBuckets_(numBuckets),
  bucketSize_(bucketSize),
  table_(new Cell[numBuckets * bucketSize]),
  overflow_()
{
  for (size_t i = 0; i < numBuckets * bucketSize; ++i)
  {
    table_[i].occupied = false;
  }
}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable& other) :
  numBuckets_(other.numBuckets_),
  bucketSize_(other.bucketSize_),
  table_(new Cell[numBuckets_ * bucketSize_]),
  overflow_(other.overflow_),
  hash_(other.hash_),
  equal_(other.equal_)
{
  for (size_t i = 0; i < numBuckets_ * bucketSize_; ++i)
  {
    table_[i] = other.table_[i];
  }
}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTable<Key, Value, Hash, Equal>::HashTable(HashTable&& other) noexcept :
  numBuckets_(0),
  bucketSize_(0),
  table_(nullptr)
{
  swap(other);
}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTable<Key, Value, Hash, Equal>::~HashTable()
{
  delete[] table_;
}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTable<Key, Value, Hash, Equal>&
velizade::HashTable<Key, Value, Hash, Equal>::operator=(const HashTable& other)
{
  if (this != &other)
  {
    HashTable tmp(other);
    swap(tmp);
  }
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTable<Key, Value, Hash, Equal>&
velizade::HashTable<Key, Value, Hash, Equal>::operator=(HashTable&& other) noexcept
{
  swap(other);
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
void velizade::HashTable<Key, Value, Hash, Equal>::swap(HashTable& other) noexcept
{
  using std::swap;
  swap(numBuckets_, other.numBuckets_);
  swap(bucketSize_, other.bucketSize_);
  swap(table_, other.table_);
  swap(overflow_, other.overflow_);
  swap(hash_, other.hash_);
  swap(equal_, other.equal_);
}

template<class Key, class Value, class Hash, class Equal>
void velizade::HashTable<Key, Value, Hash, Equal>::add(const Key& k, const Value& v)
{
  if (find(k) != nullptr)
  {
    throw std::runtime_error("Key already exists");
  }

  size_t idx = getBucketIndex(k);
  size_t start = idx * bucketSize_;
  size_t end = start + bucketSize_;
  for (size_t i = start; i < end; ++i)
  {
    if (!table_[i].occupied)
    {
      table_[i].key = k;
      table_[i].value = v;
      table_[i].occupied = true;
      return;
    }
  }
  Cell cell;
  cell.key = k;
  cell.value = v;
  cell.occupied = true;
  overflow_.pushBack(cell);
}

template<class Key, class Value, class Hash, class Equal>
Value velizade::HashTable<Key, Value, Hash, Equal>::drop(const Key& k)
{
  Cell* cell = find(k);
  if (!cell)
  {
    throw std::out_of_range("Key not found");
  }

  bool inHome = (cell >= table_ && cell < table_ + numBuckets_ * bucketSize_);
  Value val = std::move(cell->value);
  if (inHome)
  {
    cell->occupied = false;
  }
  else
  {
    for (size_t i = 0; i < overflow_.getSize(); ++i)
    {
      if (&overflow_[i] == cell)
      {
        overflow_.erase(i);
        break;
      }
    }
  }
  return val;
}

template<class Key, class Value, class Hash, class Equal>
bool velizade::HashTable<Key, Value, Hash, Equal>::has(const Key& k) const
{
  return (find(k) != nullptr);
}

template<class Key, class Value, class Hash, class Equal>
void velizade::HashTable<Key, Value, Hash, Equal>::rehash(size_t newNumBuckets)
{
  HashTable newTable(newNumBuckets, bucketSize_);
  for (size_t i = 0; i < numBuckets_ * bucketSize_; ++i)
  {
    if (table_[i].occupied)
    {
      newTable.add(table_[i].key, table_[i].value);
    }
  }
  for (size_t i = 0; i < overflow_.getSize(); ++i)
  {
    if (overflow_[i].occupied)
    {
      newTable.add(overflow_[i].key, overflow_[i].value);
    }
  }
  swap(newTable);
}

template<class Key, class Value, class Hash, class Equal>
typename velizade::HashTable<Key, Value, Hash, Equal>::iterator
velizade::HashTable<Key, Value, Hash, Equal>::begin()
{
  return iterator(this);
}

template<class Key, class Value, class Hash, class Equal>
typename velizade::HashTable<Key, Value, Hash, Equal>::iterator
velizade::HashTable<Key, Value, Hash, Equal>::end()
{
  return iterator(this, true);
}

template<class Key, class Value, class Hash, class Equal>
typename velizade::HashTable<Key, Value, Hash, Equal>::const_iterator
velizade::HashTable<Key, Value, Hash, Equal>::begin() const
{
  return const_iterator(const_cast<HashTable*>(this));
}

template<class Key, class Value, class Hash, class Equal>
typename velizade::HashTable<Key, Value, Hash, Equal>::const_iterator
velizade::HashTable<Key, Value, Hash, Equal>::end() const
{
  return const_iterator(const_cast<HashTable*>(this), true);
}

template<class Key, class Value, class Hash, class Equal>
typename velizade::HashTable<Key, Value, Hash, Equal>::Cell*
velizade::HashTable<Key, Value, Hash, Equal>::find(const Key& k)
{
  size_t idx = getBucketIndex(k);
  Cell* found = findInBucket(idx, k);
  if (found)
  {
    return found;
  }
  return findInOverflow(k);
}

template<class Key, class Value, class Hash, class Equal>
const typename velizade::HashTable<Key, Value, Hash, Equal>::Cell*
velizade::HashTable<Key, Value, Hash, Equal>::find(const Key& k) const
{
  size_t idx = getBucketIndex(k);
  const Cell* found = findInBucket(idx, k);
  if (found)
  {
    return found;
  }
  return findInOverflow(k);
}

template<class Key, class Value, class Hash, class Equal>
size_t velizade::HashTable<Key, Value, Hash, Equal>::getBucketIndex(const Key& k) const
{
  return hash_(k) % numBuckets_;
}

template<class Key, class Value, class Hash, class Equal>
typename velizade::HashTable<Key, Value, Hash, Equal>::Cell*
velizade::HashTable<Key, Value, Hash, Equal>::findInBucket(size_t idx, const Key& k)
{
  size_t start = idx * bucketSize_;
  size_t end = start + bucketSize_;
  for (size_t i = start; i < end; ++i)
  {
    if (table_[i].occupied && equal_(table_[i].key, k))
    {
      return &table_[i];
    }
  }
  return nullptr;
}

template<class Key, class Value, class Hash, class Equal>
const typename velizade::HashTable<Key, Value, Hash, Equal>::Cell*
velizade::HashTable<Key, Value, Hash, Equal>::findInBucket(size_t idx, const Key& k) const
{
  size_t start = idx * bucketSize_;
  size_t end = start + bucketSize_;
  for (size_t i = start; i < end; ++i)
  {
    if (table_[i].occupied && equal_(table_[i].key, k))
    {
      return &table_[i];
    }
  }
  return nullptr;
}

template<class Key, class Value, class Hash, class Equal>
typename velizade::HashTable<Key, Value, Hash, Equal>::Cell*
velizade::HashTable<Key, Value, Hash, Equal>::findInOverflow(const Key& k)
{
  for (size_t i = 0; i < overflow_.getSize(); ++i)
  {
    if (overflow_[i].occupied && equal_(overflow_[i].key, k))
    {
      return &overflow_[i];
    }
  }
  return nullptr;
}

template<class Key, class Value, class Hash, class Equal>
const typename velizade::HashTable<Key, Value, Hash, Equal>::Cell*
velizade::HashTable<Key, Value, Hash, Equal>::findInOverflow(const Key& k) const
{
  for (size_t i = 0; i < overflow_.getSize(); ++i)
  {
    if (overflow_[i].occupied && equal_(overflow_[i].key, k))
    {
      return &overflow_[i];
    }
  }
  return nullptr;
}

#endif
