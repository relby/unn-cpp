#pragma once
#include <string>
#include <vector>
#include <optional>
#include <set>
#include <functional>

enum TokenType {
    Invalid = 0,

    Integer,
    Identifier,
    Operation,
};

struct Token {
    TokenType type;
    std::string literal;
};

static const std::map<std::string, int> OPERATION_PRIORITY = {
    { "(", 0 },
    { ")", 1 },
    { "+", 2 },
    { "-", 2 },
    { "/", 3 },
    { "*", 3 },
    { "^", 4 },
};


class Lexer {
private:
    std::string input;
    std::string::iterator pos;

    std::optional<Token> next();
    std::optional<char> read_char();
    std::optional<char> peek_char();
    void skip_whitespace();
    std::string read_until(std::function<bool(char)> func);
public:
    Lexer(const std::string& input);
    std::vector<Token> get_tokens();
};

Lexer::Lexer(const std::string& input) {
    this->input = input;
    this->pos = this->input.begin();
}

std::vector<Token> Lexer::get_tokens() {
    std::vector<Token> tokens;
    std::optional<Token> token = this->next();
    while (token.has_value()) {
        tokens.push_back(token.value());
        token = this->next();
    }
    return tokens;
}

std::optional<Token> Lexer::next() {
    this->skip_whitespace();
    std::optional maybe_next_char = this->read_char();
    if (!maybe_next_char.has_value()) return std::nullopt;
    char next_char = maybe_next_char.value();

    TokenType type = TokenType::Invalid;
    std::string literal(1, next_char);
    if (OPERATION_PRIORITY.count(literal)) {
        type = TokenType::Operation;
        // If we had multiple char operations,
        // we wouldn't be able to handle it here
        // so keep in mind
    }
    if (std::isdigit(next_char)) {
        type = TokenType::Integer;
        literal += this->read_until([](char c) {return (bool)std::isdigit(c);});
    } else if (std::isalpha(next_char)) {
        type = TokenType::Identifier;
        literal += this->read_until([](char c) {return (bool)std::isalpha(c);});
    }
    return {
        (Token){
            .type = type,
            .literal = literal,
        }
    };
}

std::optional<char> Lexer::read_char() {
    if (pos == input.end()) return std::nullopt;
    char ch = *pos;
    std::advance(pos, 1);
    return { ch };
}

std::optional<char> Lexer::peek_char() {
    if (pos == input.end()) return std::nullopt;
    return { *pos };
}

void Lexer::skip_whitespace() {
    while (this->peek_char().has_value() && std::isspace(this->peek_char().value())) {
        this->read_char();
    }
}

std::string Lexer::read_until(std::function<bool(char)> func) {
    std::string out;
    while (this->peek_char().has_value() && func(this->peek_char().value())) {
        out.push_back(this->read_char().value());
    }
    return out;
}