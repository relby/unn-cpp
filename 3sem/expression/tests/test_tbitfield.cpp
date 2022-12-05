#include "expression.h"

#include <gtest.h>

TEST(Expression, can_create_bitfield_with_positive_length)
{
  ASSERT_NO_THROW(Expression bf("1+2+3"));
}
