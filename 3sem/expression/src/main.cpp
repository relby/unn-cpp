#include <iostream>
#include "tstack.h"
#include "expression.h"

int main() {
    // Expression e("12 + 34 + abc");
    Expression e1("5*63+(238-10)");
    std::cout << e1.get_postfix() << '\n';
    std::cout << e1.calculate() << '\n';

    Expression e2("100-a");
    std::cout << e1.get_postfix() << '\n';
    std::cout << e2.calculate({{"a", 10}}) << '\n';

    Expression e3("sin(10/3)");
    std::cout << e3.get_postfix() << '\n';
    std::cout << e3.calculate() << '\n';
}
