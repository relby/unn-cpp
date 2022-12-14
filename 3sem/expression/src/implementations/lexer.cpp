#include "lexer.h"

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
    std::optional<int> priority = std::nullopt;
    if (literal == "(") {
        type = TokenType::LeftParen;
        priority = 0;
    } else if (literal == ")") {
        type = TokenType::RightParen;
        priority = 1;
    } else if (OPERATION_PRIORITY.count(literal)) {
        type = TokenType::Operation;
        priority = OPERATION_PRIORITY.find(literal)->second;
    } else if (std::isdigit(next_char)) {
        // TODO: It still works for the case with multiple dots in a number.
        literal += this->read_until([](char c) {return (bool)std::isdigit(c) || c == '.';});
        type = TokenType::Number;
    } else if (std::isalpha(next_char)) {
        literal += this->read_until([](char c) {return (bool)std::isalpha(c);});
        if (OPERATION_PRIORITY.count(literal)) {
            type = TokenType::Operation;
            priority = OPERATION_PRIORITY.find(literal)->second;
        } else {
            type = TokenType::Identifier;
        }
    }
    return {
        (Token){
            .type = type,
            .literal = literal,
            .priority = priority,
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
