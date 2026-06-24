#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>

namespace velizade
{

  template <typename T>
  class Vector
  {
  private:
    T* data;
    size_t capacity;
    size_t size;

    void reallocate(size_t new_capacity);

  public:
    Vector();
    Vector(size_t initial_capacity);
    ~Vector();

    void push_back(const T& value);
    size_t getSize() const;
    size_t getCapacity() const;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    void clear();
    void pop_back();
    T& back();
    const T& back() const;
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;
  };
}

template <typename T>
void velizade::Vector<T>::reallocate(size_t new_capacity)
{
  T* new_data = new T[new_capacity];
  for (size_t i = 0; i < size; ++i)
  {
    new_data[i] = data[i];
  }
  delete[] data;
  data = new_data;
  capacity = new_capacity;
}

template <typename T>
velizade::Vector<T>::Vector() : data(nullptr), capacity(0), size(0) {}

template <typename T>
velizade::Vector<T>::Vector(size_t initial_capacity)
  : data(new T[initial_capacity]), capacity(initial_capacity), size(0) {}

template <typename T>
velizade::Vector<T>::~Vector()
{
  delete[] data;
}

template <typename T>
void velizade::Vector<T>::push_back(const T& value)
{
  if (size == capacity)
  {
    reallocate(capacity == 0 ? 1 : capacity * 2);
  }
  data[size++] = value;
}

template <typename T>
size_t velizade::Vector<T>::getSize() const
{
  return size;
}

template <typename T>
size_t velizade::Vector<T>::getCapacity() const
{
  return capacity;
}

template <typename T>
T& velizade::Vector<T>::operator[](size_t index)
{
  return data[index];
}

template <typename T>
const T& velizade::Vector<T>::operator[](size_t index) const
{
  return data[index];
}

template <typename T>
void velizade::Vector<T>::clear()
{
  size = 0;
}

template <typename T>
void velizade::Vector<T>::pop_back()
{
  if (size > 0)
  {
    --size;
  }
}

template <typename T>
T& velizade::Vector<T>::back()
{
  return data[size - 1];
}

template <typename T>
const T& velizade::Vector<T>::back() const
{
  return data[size - 1];
}

template <typename T>
T* velizade::Vector<T>::begin()
{
  return data;
}

template <typename T>
T* velizade::Vector<T>::end()
{
  return data + size;
}

template <typename T>
const T* velizade::Vector<T>::begin() const
{
  return data;
}

template <typename T>
const T* velizade::Vector<T>::end() const
{
  return data + size;
}

#endif
