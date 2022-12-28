#ifndef EXPRESSION_VALIDATOR_H
#define EXPRESSION_VALIDATOR_H
#include <string>
#include "lexer.h"

class ExpressionValidator {
public:
    static bool is_valid(const std::vector<Token>& tokens);
    static bool is_parens_valid(const std::vector<Token>& tokens);
    static bool is_tokens_valid(const std::vector<Token>& tokens);
};

#endif // EXPRESSION_VALIDATOR_H
