#include "expression.h"

#include <gtest.h>

TEST(Expression, is_valid) {
    Expression e1("5*63+(238-10)");
    ASSERT_TRUE(e1.is_valid());
}

TEST(Expression, can_calculate_without_variables) {
    Expression e1("5*63+(238-10)");
    ASSERT_EQ(e1.calculate(), 543);
}

TEST(Expression, can_calculate_with_variables) {
    Expression e1("aaa*63+(bbb-ccc)");
    ASSERT_EQ(
        e1.calculate({
            {"aaa", 5},
            {"bbb", 238},
            {"ccc", 10},
        }),
        543
    );
}

