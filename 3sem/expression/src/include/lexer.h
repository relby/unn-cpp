#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include <optional>
#include <set>
#include <map>
#include <functional>

enum TokenType {
    Invalid = 0,

    LeftParen,
    RightParen,

    Number,
    Identifier,
    Operation,
};

struct Token {
    TokenType type;
    std::string literal;
    std::optional<int> priority;
};

class Lexer {
private:
    std::map<std::string, int> OPERATION_PRIORITY = {
        { "+", 2 },
        { "-", 2 },
        { "/", 3 },
        { "*", 3 },
        { "^", 4 },
        { "sin", 5 },
        { "cos", 5 },
        { "neg", 5 },
    };

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

#endif // LEXER_H
