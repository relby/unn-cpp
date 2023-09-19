#ifndef TABLE_H
#define TABLE_H

#include "Iterator.h"
#include "TableRecord.h"
#include <cstddef>
#include <optional>
#include <string>
#include <iostream>
#include <fstream>
#include <type_traits>

const std::size_t DEFAULT_TABLE_CAPACITY = 1024;

template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename T>
concept Eq = requires (T x) { x == x; };

template <
    typename K,
    typename V,
    std::size_t capacity = DEFAULT_TABLE_CAPACITY
>
class Table {
protected:
    std::size_t _size;
    std::uint64_t _operations;
    Table() : _size(0), _operations(0) {}

public:
    Table(const Table&) = delete;
    Table& operator=(const Table&) = delete;
    virtual ~Table() = default;

    std::size_t size() const { return this->_size; }
    bool is_full() const { return this->_size == capacity; }
    bool is_empty() const { return this->_size == 0; }

    std::uint64_t operations() const { return this->_operations; }
    void reset_operations() { this->_operations = 0; }

    virtual std::optional<V> insert(K key, V value) = 0;
    virtual std::optional<V> get(K key) = 0;
    virtual std::optional<V> remove(K key) = 0;

    virtual bool has(K key) = 0;

    virtual Iterator<TableRecord<K, V>>& begin() = 0;
    virtual Iterator<TableRecord<K, V>>& end() = 0;

    void save_to_csv(const std::string& filepath) {
        std::ofstream file(filepath);

        for (const auto& record : *this) {
            file << record.key << ',' << record.value << '\n';
        }

        file.close();
    }

    template <typename...> requires std::is_same_v<K, std::string> && Integral<V>
    void insert_from_file(const std::string& filepath) {
        std::ifstream file(filepath);

        std::string word;
        while(file >> std::skipws >> word) {
            if (auto count = this->get(word)) {
                this->insert(word, count.value() + 1);
            } else {
                this->insert(word, 1);
            }
        }

        file.close();
    }
};

#endif // TABLE_H
