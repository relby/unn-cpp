#ifndef SCAN_TABLE_H
#define SCAN_TABLE_H

#include "ArrayTable.h"
#include <utility>
#include <fstream>

template <typename K, typename V, std::size_t capacity = DEFAULT_TABLE_CAPACITY> requires Eq<K>
class ScanTable : public ArrayTable<K, V, capacity> {
public:
    ScanTable(): ArrayTable<K, V, capacity>() {}

    std::optional<V> insert(K key, V value) override {
        for (auto& record : *this) {
            if (record.key == key) {
                return std::exchange(record.value, value);
            }
        }

        if (this->is_full()) { return std::nullopt; }

        return std::exchange(this->_data[this->_size++], TableRecord(key, value)).value;
    };

    std::optional<V> get(K key) override {
        for (const auto& record : *this) {
            if (record.key == key) {
                return record.value;
            }
        }

        return std::nullopt;
    };


    std::optional<V> remove(K key) override {
        for (auto& record : *this) {
            if (record.key == key) {
                return std::exchange(record, this->_data[--this->_size]).value;
            }
        }

        return std::nullopt;
    };

    bool has(K key) override {
        for (auto& record : *this) {
            if (record.key == key) {
                return true;
            }
        }

        return false;
    }
};

#endif // SCAN_TABLE_H
