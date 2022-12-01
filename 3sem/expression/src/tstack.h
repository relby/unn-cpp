#pragma once
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <stdexcept>

const size_t EMPTY_STACK_CAP = 4;

template<typename T>
class TStack {
private:
    T* data;
    size_t size;
    size_t cap;

    void check_for_resize();

public:
    TStack(size_t cap = EMPTY_STACK_CAP);
    TStack(const TStack<T>& stack);
    TStack(TStack<T>&& stack);
    ~TStack();

    void push(const T& value);
    const T& pop();
    bool is_empty() const;
    const T& top() const;
    size_t get_size() const;
    size_t get_cap() const;
};

template<typename T>
void TStack<T>::check_for_resize() {
    if (this->size == this->cap) {
        T* new_data = new T[this->cap * 2];
        std::copy(this->data, this->data + this->cap, new_data);
        delete[] this->data;
        this->data = new_data;
        this->cap *= 2;
    }
}

template<typename T>
TStack<T>::TStack(size_t cap)
    : size(0)
    , cap(cap)
{
    this->data = new T[this->cap];
}

template<typename T>
TStack<T>::TStack(const TStack<T>& stack) {
    if (this->cap != stack.cap) {
        this->cap = stack.cap;
        this->data = new T[this->cap];
    }
    this->size = stack.size;
    std::copy(stack.data, stack.data + stack.cap, this->data);
}

template<typename T>
TStack<T>::TStack(TStack<T>&& stack)
    : size(stack.size)
    , cap(stack.cap)
    , data(stack.data)
{
    stack.data = nullptr;
}

template<typename T>
TStack<T>::~TStack() {
    delete[] this->data;
}

template<typename T>
void TStack<T>::push(const T& value) {
    this->check_for_resize();
    this->data[size++] = value;
}

template<typename T>
const T& TStack<T>::pop() {
    if (this->is_empty()) {
        throw std::length_error("Stack is empty");
    }
    return this->data[--size];
}

template<typename T>
bool TStack<T>::is_empty() const {
    return this->size == 0;
}

template<typename T>
const T& TStack<T>::top() const {
    if (this->is_empty()) {
        throw std::length_error("Stack is empty");
    }
    return this->data[size - 1];
}

template<typename T>
size_t TStack<T>::get_size() const {
    return this->size;
}

template<typename T>
size_t TStack<T>::get_cap() const {
    return this->cap;
}