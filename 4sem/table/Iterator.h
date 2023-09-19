#ifndef ITERATOR_H
#define ITERATOR_H

template <typename T>
class Iterator {
protected:
    T* ptr;

public:
    Iterator(T& ptr): ptr(&ptr) {}
    T& operator*() { return *ptr; }
    bool operator!=(const Iterator& rhs) { return this->ptr != rhs.ptr; }

    void operator++() { ptr += 1; };
};

#endif // ITERATOR_H
