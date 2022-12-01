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

std::string join(std::vector<std::string> const &strings, std::string delim)
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
    std::vector<Token> get_postfix_tokens();
    std::string get_postfix();
    std::map<std::string, std::optional<int>> get_ident_values();
    int calculate(const std::map<std::string, int>& values = {});
};

Expression::Expression(const std::string& input)
    : infix_tokens(Lexer(input).get_tokens())
    , postfix_tokens(std::nullopt)
    , identifiers({})
{
}

std::vector<Token> Expression::get_postfix_tokens() {
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
        } else if (token.type == TokenType::Integer) {
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

int Expression::calculate(const std::map<std::string, int>& values) {
    for (const std::string& ident : this->identifiers) {
        if (!values.count(ident)) {
            throw std::invalid_argument(
                "You have to provide the value of `" + ident + "` in the argument"
            );
        }
    }
    TStack<int> stack;
    for (const Token& token : this->get_postfix_tokens()) {
        switch (token.type) {
        case TokenType::Operation: {
            int right_operand = stack.pop();
            int left_operand = stack.pop();
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
            break;
        }
        case TokenType::Integer: {
            stack.push(std::stoi(token.literal));
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