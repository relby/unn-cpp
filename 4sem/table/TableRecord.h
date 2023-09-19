#ifndef TABLE_RECORD_H
#define TABLE_RECORD_H

template<typename K, typename V>
struct TableRecord {
    K key;
    V value;

    TableRecord() = default;
    TableRecord(K key, V value): key(key), value(value) {}
};

#endif // TABLE_RECORD_H
