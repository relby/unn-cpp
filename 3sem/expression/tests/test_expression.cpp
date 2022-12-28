#include <gtest.h>
#include "expression.h"

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

TEST(Expression, throws_on_division_by_zero) {
    Expression e1("10/0");
    ASSERT_ANY_THROW(e1.calculate());
}
