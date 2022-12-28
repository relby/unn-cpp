#ifndef STACK_H
#define STACK_H
#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <utility>
#include <stdexcept>

const size_t EMPTY_STACK_CAP = 10;

template<typename T>
class Stack {
private:
    T* data;
    size_t size;
    size_t cap;

    void check_for_resize();

public:
    Stack(size_t cap = EMPTY_STACK_CAP);
    Stack(const Stack<T>& stack);
    Stack(Stack<T>&& stack);
    Stack(std::initializer_list<T> list);
    ~Stack();

    void push(const T& value);
    const T& pop();
    bool is_empty() const;
    const T& top() const;
    size_t get_size() const;
    size_t get_cap() const;
};

template<typename T>
void Stack<T>::check_for_resize() {
    if (this->size == this->cap) {
        T* new_data = new T[this->cap * 2];
        std::copy(this->data, this->data + this->cap, new_data);
        delete[] this->data;
        this->data = new_data;
        this->cap *= 2;
    }
}

template<typename T>
Stack<T>::Stack(size_t cap)
    : size(0)
    , cap(cap)
{
    this->data = new T[this->cap];
}

template<typename T>
Stack<T>::Stack(const Stack<T>& stack) {
    if (this->cap != stack.cap) {
        this->cap = stack.cap;
        this->data = new T[this->cap];
    }
    this->size = stack.size;
    std::copy(stack.data, stack.data + stack.cap, this->data);
}

template<typename T>
Stack<T>::Stack(Stack<T>&& stack)
    : size(stack.size)
    , cap(stack.cap)
    , data(stack.data)
{
    stack.data = nullptr;
}
template<typename T>
Stack<T>::Stack(std::initializer_list<T> list) : Stack(list.size()) {
    for (const T& elem : list) {
        this->push(elem);
    }
}

template<typename T>
Stack<T>::~Stack() {
    delete[] this->data;
}

template<typename T>
void Stack<T>::push(const T& value) {
    this->check_for_resize();
    this->data[size++] = value;
}

template<typename T>
const T& Stack<T>::pop() {
    if (this->is_empty()) {
        throw std::length_error("Stack is empty");
    }
    return this->data[--size];
}

template<typename T>
bool Stack<T>::is_empty() const {
    return this->size == 0;
}

template<typename T>
const T& Stack<T>::top() const {
    if (this->is_empty()) {
        throw std::length_error("Stack is empty");
    }
    return this->data[size - 1];
}

template<typename T>
size_t Stack<T>::get_size() const {
    return this->size;
}

template<typename T>
size_t Stack<T>::get_cap() const {
    return this->cap;
}
#endif // STACK_H
