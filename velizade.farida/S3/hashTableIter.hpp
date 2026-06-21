#ifndef HASHTABLEITER_HPP
#define HASHTABLEITER_HPP

#include <cstddef>
#include <utility>

namespace velizade
{
  template <class Key, class Value, class Hash, class Equal>
  class HashTable;

  template <class Key, class Value, class Hash, class Equal>
  class HTIter
  {
  public:
    using pair_t = std::pair<Key, Value>;

    HTIter() : table_(nullptr), slotIdx_(0), totalSlots_(0) {}

    pair_t& operator*() const
    {
      return table_->slots_[slotIdx_].data;
    }

    pair_t* operator->() const
    {
      return &(table_->slots_[slotIdx_].data);
    }

    HTIter& operator++()
    {
      if (table_)
      {
        do
        {
          ++slotIdx_;
        }
        while (slotIdx_ < totalSlots_ && !table_->slots_[slotIdx_].used);
        if (slotIdx_ >= totalSlots_)
        {
          slotIdx_ = totalSlots_;
        }
      }
      return *this;
    }

    HTIter operator++(int)
    {
      HTIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const HTIter& other) const
    {
      return table_ == other.table_ && slotIdx_ == other.slotIdx_;
    }

    bool operator!=(const HTIter& other) const
    {
      return !(*this == other);
    }

  private:
    friend class HashTable<Key, Value, Hash, Equal>;

    HTIter(HashTable<Key, Value, Hash, Equal>* table, size_t idx):
        table_(table),
        slotIdx_(idx),
        totalSlots_(table ? table->slots_.getSize() : 0)
    {
      if (table_ && slotIdx_ < totalSlots_ && !table_->slots_[slotIdx_].used)
      {
        ++(*this);
      }
    }

    HashTable<Key, Value, Hash, Equal>* table_;
    size_t slotIdx_;
    size_t totalSlots_;
  };

  template <class Key, class Value, class Hash, class Equal>
  class HTCiter
  {
  public:
    using pair_t = std::pair<Key, Value>;

    HTCiter() : table_(nullptr), slotIdx_(0), totalSlots_(0) {}

    HTCiter(const HTIter<Key, Value, Hash, Equal>& other):
        table_(other.table_), slotIdx_(other.slotIdx_), totalSlots_(other.totalSlots_)
    {}

    const pair_t& operator*() const
    {
      return table_->slots_[slotIdx_].data;
    }

    const pair_t* operator->() const
    {
      return &(table_->slots_[slotIdx_].data);
    }

    HTCiter& operator++()
    {
      if (table_)
      {
        do
        {
          ++slotIdx_;
        }
        while (slotIdx_ < totalSlots_ && !table_->slots_[slotIdx_].used);
        if (slotIdx_ >= totalSlots_)
        {
          slotIdx_ = totalSlots_;
        }
      }
      return *this;
    }

    HTCiter operator++(int)
    {
      HTCiter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const HTCiter& other) const
    {
      return table_ == other.table_ && slotIdx_ == other.slotIdx_;
    }

    bool operator!=(const HTCiter& other) const
    {
      return !(*this == other);
    }

  private:
    friend class HashTable<Key, Value, Hash, Equal>;

    HTCiter(const HashTable<Key, Value, Hash, Equal>* table, size_t idx):
        table_(table),
        slotIdx_(idx),
        totalSlots_(table ? table->slots_.getSize() : 0)
    {
      if (table_ && slotIdx_ < totalSlots_ && !table_->slots_[slotIdx_].used)
      {
        ++(*this);
      }
    }

    const HashTable<Key, Value, Hash, Equal>* table_;
    size_t slotIdx_;
    size_t totalSlots_;
  };
}

#endif
