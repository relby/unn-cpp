#include <gtest.h>
#include "expression_validator.h"

TEST(ExpressionValidator, is_correct)
{
    std::string expression = "6+4.5-ab*sin(x)/3-x";
    std::vector<Token> tokens = Lexer(expression).get_tokens();
    bool result = ExpressionValidator::is_valid(tokens);
    ASSERT_TRUE(result);
}

TEST(ExpressionValidator, closing_paren_not_correct)
{
    std::string expression = "(((()))5+a)";
    std::vector<Token> tokens = Lexer(expression).get_tokens();
    bool result = ExpressionValidator::is_valid(tokens);

    ASSERT_FALSE(result);
}

TEST(ExpressionValidator, opening_paren_not_correct)
{
    std::string expression = "(f+b";
    std::vector<Token> tokens = Lexer(expression).get_tokens();
    bool result = ExpressionValidator::is_valid(tokens);

    ASSERT_FALSE(result);
}

TEST(ExpressionValidator, has_invalid_tokens)
{
    std::string expression = "asdf&&&123";
    std::vector<Token> tokens = Lexer(expression).get_tokens();
    bool result = ExpressionValidator::is_valid(tokens);

    ASSERT_FALSE(result);
}





// TEST(ExpressionValidator, ExpressionIsntCorrect_ExtraBrackets)
// {
//     std::string expression = "6+4.5-a(x)/3-x";
//     std::vector<Token> tokens = Lexer(expression).get_tokens();
//     bool result = ExpressionValidator::is_valid(tokens);
//
//     ASSERT_FALSE(result);
// }

// TEST(ExpressionValidator, ExpressionIsntCorrect_EmptyBrackets1)
// {
//     std::string expression = "::";
//     std::vector<Token> tokens = Lexer(expression).get_tokens();
//     bool result = ExpressionValidator::is_valid(tokens);
//
//     ASSERT_FALSE(result);
// }
//
// TEST(ExpressionValidator, ExpressionIsntCorrect_EmptyBrackets2)
// {
//     std::string expression = "6+4.5-sin()/3-x";
//     std::vector<Token> tokens = Lexer(expression).get_tokens();
//     bool result = ExpressionValidator::is_valid(tokens);
//
//     ASSERT_FALSE(result);
// }
//
// TEST(ExpressionValidator, ExpressionIsntCorrect_EmptyBrackets3)
// {
//     std::string expression = "6+4.5-sin()a/3-x";
//     std::vector<Token> tokens = Lexer(expression).get_tokens();
//     bool result = ExpressionValidator::is_valid(tokens);
//
//     ASSERT_FALSE(result);
// }
