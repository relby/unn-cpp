#include "expression.h"

class ExpressionValidator {
public:
    static bool is_parens_valid(const Expression& expression);
    static bool has_invalid_tokens(const Expression& expression);
};

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
