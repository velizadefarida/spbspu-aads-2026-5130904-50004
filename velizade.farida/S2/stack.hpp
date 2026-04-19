#ifndef STACK_HPP
#define STACK_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace velizade {

  template<class T>
  class Stack {
  public:
    Stack() = default;
    ~Stack() = default;

    void push(const T& value);
    void push(T&& value);
    T pop();
    T& top();
    const T& top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List<T> list_;
  };

  template<class T>
  void Stack<T>::push(const T& value) {
    list_.push_front(value);
  }

  template<class T>
  void Stack<T>::push(T&& value) {
    list_.push_front(std::move(value));
  }

  template<class T>
  T Stack<T>::pop() {
    if (empty()) {
      throw std::runtime_error("Stack::pop on empty stack");
    }
    T value = std::move(list_.front());
    list_.pop_front();
    return value;
  }

  template<class T>
  T& Stack<T>::top() {
    if (empty()) {
      throw std::runtime_error("Stack::top on empty stack");
    }
    return list_.front();
  }

  template<class T>
  const T& Stack<T>::top() const {
    if (empty()) {
      throw std::runtime_error("Stack::top on empty stack");
    }
    return list_.front();
  }

  template<class T>
  bool Stack<T>::empty() const noexcept {
    return list_.empty();
  }

  template<class T>
  size_t Stack<T>::size() const noexcept {
    return list_.size();
  }
}

#endif
