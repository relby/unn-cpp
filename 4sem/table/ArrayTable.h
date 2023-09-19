#ifndef ARRAY_TABLE_H
#define ARRAY_TABLE_H

#include "Table.h"

template <typename K, typename V, std::size_t capacity = DEFAULT_TABLE_CAPACITY>
class ArrayTable : public Table<K, V, capacity> {
protected:
    TableRecord<K, V> _data[capacity];
    ArrayTable(): Table<K, V, capacity>() {}
public:
    ArrayTable(const ArrayTable&) = delete;
    ArrayTable& operator=(const ArrayTable&) = delete;

    ArrayIterator<TableRecord<K, V>>& begin() override {
        return Iterator<TableRecord<K, V>>(this->_data[0]);
    }
    ArrayIterator<TableRecord<K, V>>& end() override {
        return Iterator<TableRecord<K, V>>(this->_data[this->_size]);
    };
};

#endif // ARRAY_TABLE_H
