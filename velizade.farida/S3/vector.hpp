#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace velizade
{
  template< class T >
  class Vector
  {
  public:
    using iterator = T*;
    using const_iterator = const T*;

    Vector();
    ~Vector();

    explicit Vector(size_t size);
    Vector(size_t size, const T& value);

    Vector(const Vector< T >& rhs);
    Vector(Vector< T >&& rhs) noexcept;

    Vector< T >& operator=(const Vector< T >& rhs);
    Vector< T >& operator=(Vector< T >&& rhs) noexcept;

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void pushBack(const T&);
    void popBack();
    void pushFront(const T& v);

    void swap(Vector< T >& rhs) noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

    void insert(size_t id, const T& rhs);
    void insert(size_t id, const Vector<T>& rhs, size_t beg, size_t end);
    void erase(size_t id);
    void erase(size_t beg, size_t end);

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    iterator insert(iterator pos, const T& rhs);
    template<class InputIt>
    iterator insert(iterator pos, InputIt first, InputIt last);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

  private:
    T* data_;
    size_t size_, capacity_;
  };
}

template< class T >
velizade::Vector< T >::Vector() :
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
velizade::Vector< T >::~Vector()
{
  delete[] data_;
}

template< class T >
velizade::Vector< T >::Vector(size_t size) :
  data_(size ? new T[size] : nullptr),
  size_(size),
  capacity_(size)
{}

template< class T >
velizade::Vector< T >::Vector(size_t size, const T& value) :
  Vector(size)
{
  for (size_t i = 0; i < size; ++i)
  {
    data_[i] = value;
  }
}

template< class T >
velizade::Vector< T >::Vector(const Vector& rhs) :
  Vector(rhs.getSize())
{
  for (size_t i = 0; i < rhs.getSize(); ++i)
  {
    data_[i] = rhs.data_[i];
  }
}

template< class T >
velizade::Vector< T >::Vector(Vector&& rhs) noexcept :
  Vector()
{
  swap(rhs);
}

template< class T >
velizade::Vector< T >& velizade::Vector< T >::operator=(const Vector& rhs)
{
  if (this != &rhs)
  {
    Vector cpy(rhs);
    swap(cpy);
  }
  return *this;
}

template< class T >
velizade::Vector< T >& velizade::Vector< T >::operator=(Vector&& rhs) noexcept
{
  Vector cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template< class T >
T& velizade::Vector< T >::operator[](size_t id) noexcept
{
  return data_[id];
}

template< class T >
const T& velizade::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
bool velizade::Vector< T >::isEmpty() const noexcept
{
  return (size_ == 0);
}

template< class T >
size_t velizade::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t velizade::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void velizade::Vector< T >::pushBack(const T& value)
{
  if (size_ >= capacity_)
  {
    size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    T* newData = new T[newCapacity];
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        newData[i] = data_[i];
      }
    }
    catch (...)
    {
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }
  data_[size_++] = value;
}

template< class T >
void velizade::Vector< T >::popBack()
{
  if (size_)
  {
    --size_;
  }
}

template< class T >
void velizade::Vector< T >::pushFront(const T& value)
{
  Vector<T> v(getSize() + 1);
  v[0] = value;
  for (size_t i = 1; i < v.getSize(); ++i)
  {
    v[i] = (*this)[i - 1];
  }
  swap(v);
}

template< class T >
void velizade::Vector< T >::swap(Vector& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
T& velizade::Vector< T >::at(size_t id)
{
  if (id >= size_)
  {
    throw std::out_of_range("id out of bound");
  }
  return data_[id];
}

template< class T >
const T& velizade::Vector< T >::at(size_t id) const
{
  if (id >= size_)
  {
    throw std::out_of_range("id out of bound");
  }
  return data_[id];
}

template< class T >
void velizade::Vector< T >::insert(size_t id, const T& value)
{
  if (id > size_)
  {
    throw std::out_of_range("insert index out of range");
  }
  if (size_ == capacity_)
  {
    size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    T* newData = new T[newCapacity];
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        newData[i] = std::move(data_[i]);
      }
    }
    catch (...)
    {
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }
  for (size_t i = size_; i > id; --i)
  {
    data_[i] = std::move(data_[i - 1]);
  }
  data_[id] = value;
  ++size_;
}

template< class T >
void velizade::Vector< T >::insert(size_t id, const Vector& rhs, size_t beg, size_t end)
{
  if (id > size_)
  {
    throw std::out_of_range("insert index out of range");
  }
  if (beg > end || end > rhs.size_)
  {
    throw std::out_of_range("source range invalid");
  }
  size_t count = end - beg;
  if (count == 0)
  {
    return;
  }
  if (size_ + count > capacity_)
  {
    size_t newCapacity = size_ + count;
    if (newCapacity < capacity_ * 2)
    {
      newCapacity = capacity_ * 2;
    }
    T* newData = new T[newCapacity];
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        newData[i] = std::move(data_[i]);
      }
    }
    catch (...)
    {
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }
  for (size_t i = size_; i > id; --i)
  {
    data_[i + count - 1] = std::move(data_[i - 1]);
  }
  for (size_t i = 0; i < count; ++i)
  {
    data_[id + i] = rhs.data_[beg + i];
  }
  size_ += count;
}

template< class T >
void velizade::Vector< T >::erase(size_t id)
{
  if (id >= size_)
  {
    throw std::out_of_range("erase index out of range");
  }
  for (size_t i = id; i < size_ - 1; ++i)
  {
    data_[i] = std::move(data_[i + 1]);
  }
  --size_;
}

template< class T >
void velizade::Vector< T >::erase(size_t beg, size_t end)
{
  if (beg > end || end > size_)
  {
    throw std::out_of_range("erase range invalid");
  }
  size_t count = end - beg;
  for (size_t i = beg; i < size_ - count; ++i)
  {
    data_[i] = std::move(data_[i + count]);
  }
  size_ -= count;
}

template< class T >
typename velizade::Vector< T >::iterator velizade::Vector< T >::begin() noexcept
{
  return data_;
}

template< class T >
typename velizade::Vector< T >::const_iterator velizade::Vector< T >::begin() const noexcept
{
  return data_;
}

template< class T >
typename velizade::Vector< T >::iterator velizade::Vector< T >::end() noexcept
{
  return data_ + size_;
}

template< class T >
typename velizade::Vector< T >::const_iterator velizade::Vector< T >::end() const noexcept
{
  return data_ + size_;
}

template< class T >
typename velizade::Vector< T >::iterator velizade::Vector< T >::insert(iterator pos, const T& value)
{
  size_t index = pos - begin();
  insert(index, value);
  return begin() + index;
}

template< class T >
template< class InputIt >
typename velizade::Vector< T >::iterator velizade::Vector< T >::insert(iterator pos, InputIt first, InputIt last)
{
  size_t index = pos - begin();
  size_t count = 0;
  for (InputIt it = first; it != last; ++it)
  {
    ++count;
  }
  if (count == 0)
  {
    return begin() + index;
  }
  if (size_ + count > capacity_)
  {
    size_t newCapacity = size_ + count;
    if (newCapacity < capacity_ * 2)
    {
      newCapacity = capacity_ * 2;
    }
    T* newData = new T[newCapacity];
    try
    {
      for (size_t i = 0; i < size_; ++i)
      {
        newData[i] = std::move(data_[i]);
      }
    }
    catch (...)
    {
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }
  for (size_t i = size_; i > index; --i)
  {
    data_[i + count - 1] = std::move(data_[i - 1]);
  }
  InputIt it = first;
  for (size_t i = 0; i < count; ++i, ++it)
  {
    data_[index + i] = *it;
  }
  size_ += count;
  return begin() + index;
}

template< class T >
typename velizade::Vector< T >::iterator velizade::Vector< T >::erase(iterator pos)
{
  size_t index = pos - begin();
  erase(index);
  return begin() + index;
}

template< class T >
typename velizade::Vector< T >::iterator velizade::Vector< T >::erase(iterator first, iterator last)
{
  size_t beg = first - begin();
  size_t end = last - begin();
  erase(beg, end);
  return begin() + beg;
}

#endif
