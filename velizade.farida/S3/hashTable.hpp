#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <utility>
#include <stdexcept>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hash_append.hpp>
#include "vector.hpp"
#include "hashTableIter.hpp"

namespace velizade
{
  template <class T>
  struct SipHashFunctor
  {
    std::size_t operator()(const T& val) const
    {
      boost::hash2::siphash_64 hasher;
      boost::hash2::hash_append(hasher, {}, val);
      return hasher.result();
    }
  };

  template <class T>
  struct EqualFunctor
  {
    bool operator()(const T& lhs, const T& rhs) const
    {
      return lhs == rhs;
    }
  };

  template <class Key, class Value, class Hash = SipHashFunctor<Key>, class Equal = EqualFunctor<Key>>
  class HashTable
  {
  public:
    explicit HashTable(size_t buckets = 16, size_t bucketCap = 4, size_t overflowCap = 4);

    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    ~HashTable() = default;

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    void swap(HashTable& other) noexcept;

    void add(const Key& k, const Value& v);
    bool drop(const Key& k);
    bool has(const Key& k) const;

    HTIter<Key, Value, Hash, Equal> find(const Key& k);
    HTCiter<Key, Value, Hash, Equal> find(const Key& k) const;

    void rehash(size_t newBuckets);

    size_t getSize() const noexcept
    {
      return size_;
    }

    size_t getBucketCount() const noexcept
    {
      return buckets_;
    }

    size_t getBucketCapacity() const noexcept
    {
      return bucketCapacity_;
    }

    size_t getOverflowCapacity() const noexcept
    {
      return overflowCapacity_;
    }

    HTIter<Key, Value, Hash, Equal> begin();
    HTIter<Key, Value, Hash, Equal> end();

    HTCiter<Key, Value, Hash, Equal> begin() const;
    HTCiter<Key, Value, Hash, Equal> end() const;
    HTCiter<Key, Value, Hash, Equal> cbegin() const;
    HTCiter<Key, Value, Hash, Equal> cend() const;

  private:
    struct Slot
    {
      std::pair<Key, Value> data;
      bool used;
      Slot() : used(false) {}
    };

    Vector<Slot> slots_;
    size_t buckets_;
    size_t bucketCapacity_;
    size_t overflowCapacity_;
    size_t size_;

    Hash hashFn_;
    Equal equalFn_;

    size_t bucketIndex(const Key& k) const;
    Slot* findSlot(const Key& k, size_t& outBucketIdx, size_t& outSlotIdx) const;
    Slot* findFreeInBucket(size_t bucketIdx) const;
    Slot* findFreeInOverflow() const;

    friend class HTIter<Key, Value, Hash, Equal>;
    friend class HTCiter<Key, Value, Hash, Equal>;
  };

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::HashTable(size_t buckets, size_t bucketCap, size_t overflowCap):
      buckets_(buckets),
      bucketCapacity_(bucketCap),
      overflowCapacity_(overflowCap),
      size_(0)
  {
    if (buckets_ == 0 || bucketCapacity_ == 0 || overflowCapacity_ == 0)
    {
      throw std::invalid_argument("HashTable: capacities must be > 0");
    }

    size_t total = buckets_ * bucketCapacity_ + overflowCapacity_;
    slots_ = Vector<Slot>(total);
    for (size_t i = 0; i < total; ++i)
    {
      new (&slots_[i]) Slot();
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable& other):
      slots_(other.slots_),
      buckets_(other.buckets_),
      bucketCapacity_(other.bucketCapacity_),
      overflowCapacity_(other.overflowCapacity_),
      size_(other.size_),
      hashFn_(other.hashFn_),
      equalFn_(other.equalFn_)
  {}

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::HashTable(HashTable&& other) noexcept:
      slots_(),
      buckets_(0),
      bucketCapacity_(0),
      overflowCapacity_(0),
      size_(0)
  {
    swap(other);
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>&
  HashTable<Key, Value, Hash, Equal>::operator=(const HashTable& other)
  {
    if (this != &other)
    {
      HashTable tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>&
  HashTable<Key, Value, Hash, Equal>::operator=(HashTable&& other) noexcept
  {
    if (this != &other)
    {
      HashTable tmp(std::move(other));
      swap(tmp);
    }
    return *this;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::swap(HashTable& other) noexcept
  {
    slots_.swap(other.slots_);
    std::swap(buckets_, other.buckets_);
    std::swap(bucketCapacity_, other.bucketCapacity_);
    std::swap(overflowCapacity_, other.overflowCapacity_);
    std::swap(size_, other.size_);
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::bucketIndex(const Key& k) const
  {
    return hashFn_(k) % buckets_;
  }

  template <class Key, class Value, class Hash, class Equal>
  typename HashTable<Key, Value, Hash, Equal>::Slot*
  HashTable<Key, Value, Hash, Equal>::findSlot(const Key& k, size_t& outBucketIdx, size_t& outSlotIdx) const
  {
    size_t idx = bucketIndex(k);
    outBucketIdx = idx;

    size_t start = idx * bucketCapacity_;
    size_t end = start + bucketCapacity_;
    for (size_t i = start; i < end; ++i)
    {
      if (slots_[i].used && equalFn_(slots_[i].data.first, k))
      {
        outSlotIdx = i;
        return const_cast<Slot*>(&slots_[i]);
      }
    }

    size_t overflowStart = buckets_ * bucketCapacity_;
    size_t overflowEnd = overflowStart + overflowCapacity_;
    for (size_t i = overflowStart; i < overflowEnd; ++i)
    {
      if (slots_[i].used && equalFn_(slots_[i].data.first, k))
      {
        outSlotIdx = i;
        return const_cast<Slot*>(&slots_[i]);
      }
    }

    return nullptr;
  }

  template <class Key, class Value, class Hash, class Equal>
  typename HashTable<Key, Value, Hash, Equal>::Slot*
  HashTable<Key, Value, Hash, Equal>::findFreeInBucket(size_t bucketIdx) const
  {
    size_t start = bucketIdx * bucketCapacity_;
    size_t end = start + bucketCapacity_;
    for (size_t i = start; i < end; ++i)
    {
      if (!slots_[i].used)
      {
        return const_cast<Slot*>(&slots_[i]);
      }
    }
    return nullptr;
  }

  template <class Key, class Value, class Hash, class Equal>
  typename HashTable<Key, Value, Hash, Equal>::Slot*
  HashTable<Key, Value, Hash, Equal>::findFreeInOverflow() const
  {
    size_t start = buckets_ * bucketCapacity_;
    size_t end = start + overflowCapacity_;
    for (size_t i = start; i < end; ++i)
    {
      if (!slots_[i].used)
      {
        return const_cast<Slot*>(&slots_[i]);
      }
    }
    return nullptr;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::add(const Key& k, const Value& v)
  {
    size_t unusedIdx, unusedSlot;
    Slot* found = findSlot(k, unusedIdx, unusedSlot);
    if (found)
    {
      found->data.second = v;
      return;
    }

    size_t idx = bucketIndex(k);
    Slot* freeSlot = findFreeInBucket(idx);
    if (!freeSlot)
    {
      freeSlot = findFreeInOverflow();
    }

    if (!freeSlot)
    {
      throw std::overflow_error("HashTable: no free slot");
    }

    freeSlot->data.first = k;
    freeSlot->data.second = v;
    freeSlot->used = true;
    ++size_;
  }

  template <class Key, class Value, class Hash, class Equal>
  bool HashTable<Key, Value, Hash, Equal>::drop(const Key& k)
  {
    size_t idx, slotIdx;
    Slot* found = findSlot(k, idx, slotIdx);
    if (!found)
    {
      return false;
    }

    found->used = false;
    --size_;
    return true;
  }

  template <class Key, class Value, class Hash, class Equal>
  bool HashTable<Key, Value, Hash, Equal>::has(const Key& k) const
  {
    size_t idx, slotIdx;
    return findSlot(k, idx, slotIdx) != nullptr;
  }

  template <class Key, class Value, class Hash, class Equal>
  HTIter<Key, Value, Hash, Equal>
  HashTable<Key, Value, Hash, Equal>::find(const Key& k)
  {
    size_t idx, slotIdx;
    Slot* found = findSlot(k, idx, slotIdx);
    if (found)
    {
      return HTIter<Key, Value, Hash, Equal>(this, slotIdx);
    }
    return end();
  }

  template <class Key, class Value, class Hash, class Equal>
  HTCiter<Key, Value, Hash, Equal>
  HashTable<Key, Value, Hash, Equal>::find(const Key& k) const
  {
    size_t idx, slotIdx;
    Slot* found = findSlot(k, idx, slotIdx);
    if (found)
    {
      return HTCiter<Key, Value, Hash, Equal>(this, slotIdx);
    }
    return cend();
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::rehash(size_t newBuckets)
  {
    if (newBuckets == 0)
    {
      throw std::invalid_argument("HashTable::rehash: newBuckets must be > 0");
    }

    size_t oldBucketCap = bucketCapacity_;
    size_t oldOverflowCap = overflowCapacity_;
    Vector<Slot> oldSlots = std::move(slots_);

    HashTable newTable(newBuckets, oldBucketCap, oldOverflowCap);

    size_t totalOld = oldSlots.getSize();
    for (size_t i = 0; i < totalOld; ++i)
    {
      if (oldSlots[i].used)
      {
        newTable.add(oldSlots[i].data.first, oldSlots[i].data.second);
      }
    }

    swap(newTable);
  }

  template <class Key, class Value, class Hash, class Equal>
  HTIter<Key, Value, Hash, Equal>
  HashTable<Key, Value, Hash, Equal>::begin()
  {
    return HTIter<Key, Value, Hash, Equal>(this, 0);
  }

  template <class Key, class Value, class Hash, class Equal>
  HTIter<Key, Value, Hash, Equal>
  HashTable<Key, Value, Hash, Equal>::end()
  {
    return HTIter<Key, Value, Hash, Equal>(this, slots_.getSize());
  }

  template <class Key, class Value, class Hash, class Equal>
  HTCiter<Key, Value, Hash, Equal>
  HashTable<Key, Value, Hash, Equal>::begin() const
  {
    return cbegin();
  }

  template <class Key, class Value, class Hash, class Equal>
  HTCiter<Key, Value, Hash, Equal>
  HashTable<Key, Value, Hash, Equal>::end() const
  {
    return cend();
  }

  template <class Key, class Value, class Hash, class Equal>
  HTCiter<Key, Value, Hash, Equal>
  HashTable<Key, Value, Hash, Equal>::cbegin() const
  {
    return HTCiter<Key, Value, Hash, Equal>(this, 0);
  }

  template <class Key, class Value, class Hash, class Equal>
  HTCiter<Key, Value, Hash, Equal>
  HashTable<Key, Value, Hash, Equal>::cend() const
  {
    return HTCiter<Key, Value, Hash, Equal>(this, slots_.getSize());
  }
}

#endif
