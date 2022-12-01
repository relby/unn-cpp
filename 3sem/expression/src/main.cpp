#include <iostream>
#include "tstack.h"
#include "lexer.h"

int main() {
    // TStack<int> t;
    // t.push(10);
    // t.push(20);
    // t.push(30);
    // t.push(40);
    // t.push(50);
    // std::cout << t.get_size() << ' ' << t.get_cap() << '\n';
    // while (!t.is_empty()) {
    //     std::cout << t.pop() << '\n';
    // }

    Lexer l("sin(12 + 34)");
    auto t = l.get_tokens();
    for (auto& x : t) {
        std::cout << x.type << ' ' << x.literal << '\n';
    }
    auto t1 = l.get_tokens();
    for (auto& x : t1) {
        std::cout << x.type << ' ' << x.literal << '\n';
    }
    // std::cout << l.next().literal << '\n';
    // std::cout << l.next().literal << '\n';
    // std::cout << l.next().literal << '\n';
    // std::cout << l.next().literal << '\n';

}
