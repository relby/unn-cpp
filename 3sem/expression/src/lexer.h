#pragma once
#include <vector>
#include <optional>
#include <set>

enum TokenType {
    Illegal = 0,

    // Operations
    Plus,
    Minus,
    LeftParen,
    RightParen,

    // Multichar
    Integer,
    Ident,
    Function,

    // Special
    End,
};

std::set<std::string> functions = {
    "sin",
};

struct Token {
    TokenType type;
    std::string literal;
};

class Lexer {
private:
    std::string input;
    std::string::iterator pos;

    Token next();
    std::optional<char> read_char();
    std::optional<char> peek_char();
    void skip_whitespace();
    std::string read_until(bool (*func)(char));
public:
    Lexer(const std::string& input);
    bool is_input_valid();
    std::vector<Token> get_tokens();
};

Lexer::Lexer(const std::string& input) {
    this->input = input;
    this->pos = this->input.begin();
}

std::vector<Token> Lexer::get_tokens() {
    std::vector<Token> tokens;
    Token token = this->next();
    while (token.type != TokenType::End) {
        tokens.push_back(token);
        token = this->next();
    }
    this->pos = this->input.begin();
    return tokens;
}

bool Lexer::is_input_valid() {
    for (const Token& token : this->get_tokens()) {
        if (token.type == TokenType::Illegal) return false;
    }
    return true;
}

Token Lexer::next() {
    this->skip_whitespace();
    std::optional maybe_next_char = this->read_char();
    if (!maybe_next_char.has_value()) {
        return (Token){
            .type = TokenType::End,
            .literal = "",
        };
    }
    char next_char = maybe_next_char.value();
    TokenType type = TokenType::Illegal;
    std::string literal(1, next_char);
    switch (next_char) {
    case '+': type = TokenType::Plus; break;
    case '-': type = TokenType::Minus; break;
    case '(': type = TokenType::LeftParen; break;
    case ')': type = TokenType::RightParen; break;
    default:
        if (std::isdigit(next_char)) {
            literal += this->read_until([](char c) {return (bool)std::isdigit(c);});
            type = TokenType::Integer;
        } else if (std::isalpha(next_char)) {
            literal += this->read_until([](char c) {return (bool)std::isalpha(c);});
            type = functions.count(literal) ? TokenType::Function : TokenType::Ident;
        }
    }
    return (Token){
        .type = type,
        .literal = literal,
    };
}

std::optional<char> Lexer::read_char() {
    if (pos == input.end()) return std::nullopt;
    char ch = *pos;
    std::advance(pos, 1);
    return ch;
}

std::optional<char> Lexer::peek_char() {
    if (pos == input.end()) return std::nullopt;
    return *pos;
}

void Lexer::skip_whitespace() {
    while (this->peek_char().has_value() && std::isspace(this->peek_char().value())) {
        this->read_char();
    }
}

std::string Lexer::read_until(bool (*func)(char)) {
    std::string out;
    while (this->peek_char().has_value() && func(this->peek_char().value())) {
        out.push_back(this->read_char().value());
    }
    return out;
}