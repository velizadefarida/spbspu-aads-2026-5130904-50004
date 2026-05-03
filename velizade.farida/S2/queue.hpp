#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace velizade {

  template<class T>
  class Queue {
  public:
    Queue() = default;
    ~Queue() = default;

    void push(const T& value);
    void push(T&& value);
    T pop();
    T& front();
    const T& front() const;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List<T> list_;
  };

  template<class T>
  void Queue<T>::push(const T& value) {
    list_.push_back(value);
  }

  template<class T>
  void Queue<T>::push(T&& value) {
    list_.push_back(std::move(value));
  }

  template<class T>
  T Queue<T>::pop() {
    if (empty()) {
      throw std::runtime_error("Queue::pop on empty queue");
    }
    T value = std::move(list_.front());
    list_.pop_front();
    return value;
  }

  template<class T>
  T& Queue<T>::front() {
    if (empty()) {
      throw std::runtime_error("Queue::front on empty queue");
    }
    return list_.front();
  }

  template<class T>
  const T& Queue<T>::front() const {
    if (empty()) {
      throw std::runtime_error("Queue::front on empty queue");
    }
    return list_.front();
  }

  template<class T>
  bool Queue<T>::empty() const noexcept {
    return list_.empty();
  }

  template<class T>
  size_t Queue<T>::size() const noexcept {
    return list_.size();
  }
}

#endif
