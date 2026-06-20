#ifndef HASH_TABLE_ITERATOR_HPP
#define HASH_TABLE_ITERATOR_HPP

#include <utility>
#include <cstddef>

namespace velizade
{
  template<class Key, class Value, class Hash, class Equal>
  class HashTable;

  template<class Key, class Value, class Hash, class Equal>
  class HashTableIterator
  {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::pair<const Key, Value>;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::forward_iterator_tag;

    HashTableIterator();
    explicit HashTableIterator(HashTable<Key, Value, Hash, Equal>* tbl, bool end = false);

    HashTableIterator& operator++();
    bool operator==(const HashTableIterator& other) const;
    bool operator!=(const HashTableIterator& other) const;
    std::pair<Key, Value> operator*() const;

  private:
    using HashTableType = HashTable<Key, Value, Hash, Equal>;
    HashTableType* table_;
    size_t homeIndex_;
    size_t overflowIndex_;
    bool inOverflow_;

    void advanceToNextOccupied();
  };

}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTableIterator<Key, Value, Hash, Equal>::HashTableIterator() :
  table_(nullptr),
  homeIndex_(0),
  overflowIndex_(0),
  inOverflow_(false)
{
}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTableIterator<Key, Value, Hash, Equal>::HashTableIterator(HashTableType* tbl, bool end) :
  table_(tbl)
{
  if (!end)
  {
    homeIndex_ = 0;
    inOverflow_ = false;
    advanceToNextOccupied();
  }
  else
  {
    homeIndex_ = tbl->numBuckets_ * tbl->bucketSize_;
    overflowIndex_ = tbl->overflow_.getSize();
    inOverflow_ = true;
  }
}

template<class Key, class Value, class Hash, class Equal>
velizade::HashTableIterator<Key, Value, Hash, Equal>&
velizade::HashTableIterator<Key, Value, Hash, Equal>::operator++()
{
  advanceToNextOccupied();
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
bool velizade::HashTableIterator<Key, Value, Hash, Equal>::operator==(const HashTableIterator& other) const
{
  if (table_ != other.table_)
  {
    return false;
  }
  if (inOverflow_ != other.inOverflow_)
  {
    return false;
  }
  if (inOverflow_)
  {
    return (overflowIndex_ == other.overflowIndex_);
  }
  else
  {
    return (homeIndex_ == other.homeIndex_);
  }
}

template<class Key, class Value, class Hash, class Equal>
bool velizade::HashTableIterator<Key, Value, Hash, Equal>::operator!=(const HashTableIterator& other) const
{
  return !(*this == other);
}

template<class Key, class Value, class Hash, class Equal>
std::pair<Key, Value> velizade::HashTableIterator<Key, Value, Hash, Equal>::operator*() const
{
  if (inOverflow_)
  {
    return {table_->overflow_[overflowIndex_].key, table_->overflow_[overflowIndex_].value};
  }
  else
  {
    return {table_->table_[homeIndex_].key, table_->table_[homeIndex_].value};
  }
}

template<class Key, class Value, class Hash, class Equal>
void velizade::HashTableIterator<Key, Value, Hash, Equal>::advanceToNextOccupied()
{
  if (!inOverflow_)
  {
    size_t total = table_->numBuckets_ * table_->bucketSize_;
    while (homeIndex_ < total && !table_->table_[homeIndex_].occupied)
    {
      ++homeIndex_;
    }
    if (homeIndex_ < total)
    {
      return;
    }
    inOverflow_ = true;
    overflowIndex_ = 0;
    while (overflowIndex_ < table_->overflow_.getSize() &&
           !table_->overflow_[overflowIndex_].occupied)
    {
      ++overflowIndex_;
    }
  }
  else
  {
    ++overflowIndex_;
    while (overflowIndex_ < table_->overflow_.getSize() &&
           !table_->overflow_[overflowIndex_].occupied)
    {
      ++overflowIndex_;
    }
  }
}

#endif
