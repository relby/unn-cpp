#pragma once
#include <string>
#include <map>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "tstack.h"
#include "lexer.h"

std::string join(const std::vector<std::string> &strings, const std::string& delim)
{
    std::stringstream ss;
    std::copy(strings.begin(), strings.end(),
        std::ostream_iterator<std::string>(ss, delim.c_str()));
    return ss.str();
}

class Expression {
private:
    std::vector<Token> infix_tokens;
    std::optional<std::vector<Token>> postfix_tokens;
    std::set<std::string> identifiers;
public:
    Expression(const std::string& input);

    // Getters
    const std::vector<Token>& get_infix_tokens();
    std::string get_infix();
    const std::vector<Token>& get_postfix_tokens();
    std::string get_postfix();
    const std::set<std::string>& get_identifiers();

    double calculate(const std::map<std::string, double>& values = {});
    bool is_parens_valid();
};

Expression::Expression(const std::string& input)
    : infix_tokens(Lexer(input).get_tokens())
    , postfix_tokens(std::nullopt)
    , identifiers({})
{
}


const std::vector<Token>& Expression::get_infix_tokens() {
    return this->infix_tokens;
}

std::string Expression::get_infix() {
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
    TStack<Token> stack;
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

const std::set<std::string>& Expression::get_identifiers() {
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
    TStack<double> stack;
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

bool Expression::is_parens_valid() {
    int count = 0;
    for (const Token& token : this->get_infix_tokens()) {
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

