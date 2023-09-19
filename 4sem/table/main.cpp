#include "ScanTable.h"

#include <string>
#include <iostream>

int main() {
    ScanTable<std::string, int> table;
    table.insert_from_file("text.txt");
    table.save_to_csv("test.csv");
}

int main1() {
    ScanTable<std::string, int> table;
    table.insert("asdf", 1);
    table.insert("rew", 2);
    table.insert("asdf", 3);

    for (auto& x : table) {
        std::cout << x.key << "," << x.value << "\n";
    }

    table.save_to_csv("test.csv");


    if (auto x = table.get("asdf")) {
        std::cout << x.value() << '\n';
    }

    return 0;

    // std::cout << table.get().value() << '\n';
}
