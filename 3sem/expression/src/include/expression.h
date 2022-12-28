#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <stdexcept>
#include <string>
#include <map>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <exception>
#include "stack.h"
#include "lexer.h"
#include "expression_validator.h"

class Expression {
private:
    std::vector<Token> infix_tokens;
    std::optional<std::vector<Token>> postfix_tokens;
    std::set<std::string> identifiers;
public:
    Expression(const std::string& input);

    // Getters
    const std::vector<Token>& get_infix_tokens() const;
    std::string get_infix() const;
    const std::vector<Token>& get_postfix_tokens();
    std::string get_postfix();
    const std::set<std::string>& get_identifiers() const;

    double calculate(const std::map<std::string, double>& values = {});
};
#endif // EXPRESSION_H
