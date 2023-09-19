#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include <cstdint>
#include <stdexcept>

template <typename T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& value): value(value), next(nullptr) {}
    };
    size_t _size;
    Node* _head;
    Node* _tail;
public:
    Queue();
    void push(const T& value);
    T pop();
    const T& top() const;
    size_t size() const;
    bool is_empty() const;
    void print() const;
    std::vector<T> to_vector() const;
};

template<typename T>
Queue<T>::Queue()
    : _size(0)
    , _head(nullptr)
    , _tail(nullptr)
{}

template <typename T>
void Queue<T>::push(const T& value) {
    Node* node = new Node(value);

    if (this->is_empty()) {
        this->_tail = node;
        this->_head = node;
    } else {
        this->_tail->next = node;
        this->_tail = node;
    }

    this->_size += 1;
}

template <typename T>
T Queue<T>::pop() {
    if (this->is_empty()) {
        throw std::out_of_range("Queue is empty");
    }

    Node* node = this->_head;

    this->_head = this->_head->next;
    if (this->_head == nullptr) {
        this->_tail = nullptr;
    }
    this->_size -= 1;

    T value = node->value;
    delete node;
    return value;
}

template <typename T>
const T& Queue<T>::top() const {
    if (this->is_empty()) {
        throw std::out_of_range("Queue is empty");
    }
    return this->_head->value;
}

template <typename T>
size_t Queue<T>::size() const {
    return this->_size;
}
template <typename T>
bool Queue<T>::is_empty() const {
    return this->_size == 0;
}

template <typename T>
void Queue<T>::print() const {
    Node* tmp = this->_head;

    while (tmp) {
        std::cout << tmp->value << ' ';
        tmp = tmp->next;
    }
    std::cout << '\n';
}

template <typename T>
std::vector<T> Queue<T>::to_vector() const {
    std::vector<T> out;
    Node* ptr = this->_head;
    while (ptr != nullptr) {
        out.push_back(ptr->value);
        ptr = ptr->next;
    }
    return out;
}
#endif // QUEUE_H
