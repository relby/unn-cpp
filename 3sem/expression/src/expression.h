#pragma once
#include <stdexcept>
#include <string>
#include <map>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <exception>
#include "tstack.h"
#include "lexer.h"

std::string join(const std::vector<std::string> &strings, const std::string& delim)
{
    std::stringstream ss;
    std::copy(strings.begin(), strings.end(),
        std::ostream_iterator<std::string>(ss, delim.c_str()));
    return ss.str();
}

// TODO: Move implementations to separate files

// Declaration
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

class ExpressionValidator {
public:
    static bool is_parens_valid(const Expression& expression);
    static bool has_invalid_tokens(const Expression& expression);
};

// Implementation
Expression::Expression(const std::string& input)
    : infix_tokens(Lexer(input).get_tokens())
    , postfix_tokens(std::nullopt)
    , identifiers({})
{
    if (!ExpressionValidator::is_parens_valid(*this)) {
        throw std::logic_error("Parens are not valid");
    }
    if (!ExpressionValidator::has_invalid_tokens(*this)) {
        throw std::logic_error("Expression has invalid tokens");
    }
}


const std::vector<Token>& Expression::get_infix_tokens() const {
    return this->infix_tokens;
}

std::string Expression::get_infix() const {
    std::string out;
    for (const Token& token : this->get_infix_tokens()) {
        out += token.literal;
    }
    return out;
}

const std::vector<Token>& Expression::get_postfix_tokens() {
    if (this->postfix_tokens.has_value()) {
        return this->postfix_tokens.value();
    }
    this->postfix_tokens = std::make_optional<std::vector<Token>>({});
    Stack<Token> stack;
    for (const Token& token : this->infix_tokens) {
        // TODO: consider introducing separate TokenType for left and right paren
        if (token.literal == "(") {
            stack.push(token);
        } else if (token.literal == ")") {
            Token item = stack.pop();
            while (item.literal != "(") {
                this->postfix_tokens.value().push_back(item);
                item = stack.pop();
            }
        } else if (token.type == TokenType::Operation) {
            while (!stack.is_empty()) {
                Token item = stack.pop();
                if (OPERATION_PRIORITY.at(token.literal) <= OPERATION_PRIORITY.at(item.literal)) {
                    this->postfix_tokens.value().push_back(item);
                } else {
                    stack.push(item);
                    break;
                }
            }
            stack.push(token);
        } else if (token.type == TokenType::Number) {
            this->postfix_tokens.value().push_back(token);
        } else if (token.type == TokenType::Identifier) {
            this->identifiers.insert(token.literal);
            this->postfix_tokens.value().push_back(token);
        }
    }
    while(!stack.is_empty()) {
        this->postfix_tokens.value().push_back(stack.pop());
    }
    return this->postfix_tokens.value();
}

std::string Expression::get_postfix() {
    std::vector<std::string> out;
    for (const Token& token : this->get_postfix_tokens()) {
        out.push_back(token.literal);
    }
    return join(out, " ");
}

const std::set<std::string>& Expression::get_identifiers() const {
    return this->identifiers;
}

double Expression::calculate(const std::map<std::string, double>& values) {
    for (const std::string& ident : this->identifiers) {
        if (!values.count(ident)) {
            throw std::invalid_argument(
                "You have to provide the value of `" + ident + "` in the argument"
            );
        }
    }
    Stack<double> stack;
    for (const Token& token : this->get_postfix_tokens()) {
        switch (token.type) {
        case TokenType::Operation: {
            if (token.literal == "sin") {
                double operand = stack.pop();
                stack.push(std::sin(operand));
            } else if (token.literal == "cos") {
                double operand = stack.pop();
                stack.push(std::cos(operand));
            } else if (token.literal == "neg") {
                double operand = stack.pop();
                stack.push(-operand);
            } else {
                double right_operand = stack.pop();
                double left_operand = stack.pop();
                if (token.literal == "+") {
                    stack.push(left_operand + right_operand);
                } else if (token.literal == "-") {
                    stack.push(left_operand - right_operand);
                } else if (token.literal == "*") {
                    stack.push(left_operand * right_operand);
                } else if (token.literal == "/") {
                    if (right_operand == 0) {
                        throw std::logic_error("Can't divide by zero");
                    }
                    stack.push(left_operand / right_operand);
                } else if (token.literal == "^") {
                    stack.push(std::pow(left_operand, right_operand));
                }
            }
            break;
        }
        case TokenType::Number: {
            stack.push(std::stod(token.literal));
            break;
        }
        case TokenType::Identifier: {
            stack.push(values.at(token.literal));
            break;
        }
        }
    }
    return stack.pop();
}


bool ExpressionValidator::is_parens_valid(const Expression& expression) {
    int count = 0;
    for (const Token& token : expression.get_infix_tokens()) {
        if (token.literal == "(") {
            count += 1;
        } else if (token.literal == ")") {
            count -= 1;
        }
        if (count < 0) {
            return false;
        }
    }
    return true;
}

bool ExpressionValidator::has_invalid_tokens(const Expression& expression) {
    for (const Token& token : expression.get_infix_tokens()) {
        if (token.type == TokenType::Invalid) {
            return true;
        }
    }
    return false;
}
