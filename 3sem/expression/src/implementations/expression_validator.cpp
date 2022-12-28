#include "stack.h"
#include "expression_validator.h"

bool ExpressionValidator::is_valid(const std::vector<Token>& tokens) {
    return ExpressionValidator::is_parens_valid(tokens) &&
        ExpressionValidator::is_tokens_valid(tokens);
}

bool ExpressionValidator::is_parens_valid(const std::vector<Token>& tokens) {
    int count = 0;
    bool prev_left_paren = false;
    for (const Token& token : tokens) {
        if (token.type == TokenType::LeftParen) {
            count += 1;
            prev_left_paren = true;
        } else if (token.type == TokenType::RightParen) {
            if (prev_left_paren) {
                return false;
            }
            count -= 1;
            prev_left_paren = false;
        } else {
            prev_left_paren = false;
        }
        if (count < 0) {
            return false;
        }
    }
    return count == 0;
}

bool ExpressionValidator::is_tokens_valid(const std::vector<Token>& tokens) {
    for (const Token& token : tokens) {
        if (token.type == TokenType::Invalid) {
            return false;
        }
    }
    return true;
}
