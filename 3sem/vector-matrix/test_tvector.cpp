#include "utmatrix.h"

#include <gtest.h>

TEST(TVector, can_create_vector_with_positive_length)
{
  ASSERT_NO_THROW(TVector<int> v(5));
}

TEST(TVector, cant_create_too_large_vector)
{
  ASSERT_ANY_THROW(TVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TVector, throws_when_create_vector_with_negative_length)
{
  ASSERT_ANY_THROW(TVector<int> v(-5));
}

TEST(TVector, throws_when_create_vector_with_negative_startindex)
{
  ASSERT_ANY_THROW(TVector<int> v(5, -2));
}

TEST(TVector, can_create_copied_vector)
{
  TVector<int> v(10);

  ASSERT_NO_THROW(TVector<int> v1(v));
}

TEST(TVector, copied_vector_is_equal_to_source_one)
{
  const int size = 10;
  TVector<int> v1(size);
  for (int i = 0; i < size; i++)
    v1[i] = i;
  TVector<int> v2(v1);
  EXPECT_EQ(v1, v2);
}

TEST(TVector, copied_vector_has_its_own_memory)
{
  const int size = 10;
  TVector<int> *v1 = new TVector<int>(size);
  for (int i = 0; i < size; i++)
    (*v1)[i] = i;
  TVector<int> v2(*v1);
  delete v1;
  EXPECT_EQ(0, v2[0]);
  EXPECT_EQ(size - 1, v2[size - 1]);
}

TEST(TVector, can_get_size)
{
  TVector<int> v(4);

  EXPECT_EQ(4, v.GetSize());
}

TEST(TVector, can_get_start_index)
{
  TVector<int> v(4, 2);

  EXPECT_EQ(2, v.GetStartIndex());
}

TEST(TVector, can_set_and_get_element)
{
  TVector<int> v(4);
  v[0] = 4;

  EXPECT_EQ(4, v[0]);
}

TEST(TVector, throws_when_set_element_with_negative_index)
{
  TVector<int> v(5);
  ASSERT_ANY_THROW(v[-1] = 4);
}

TEST(TVector, throws_when_set_element_with_too_large_index)
{
  TVector<int> v(5, 2);
  ASSERT_ANY_THROW(v[7] = 4);
}

TEST(TVector, can_assign_vector_to_itself)
{
  TVector<int> v(4);
  ASSERT_NO_THROW(v = v;);
}

TEST(TVector, can_assign_vectors_of_equal_size)
{
  const int size = 4;
  TVector<int> v1(size), v2(size);
  for (int i = 0; i < size; i++)
    v1[i] = i;
  v2 = v1;
  EXPECT_EQ(0, v2[0]);
  EXPECT_EQ(size - 1, v2[size - 1]);
}

TEST(TVector, assign_operator_change_vector_size)
{
  const int size1 = 4, size2 = 5;
  TVector<int> v1(size1), v2(size2);
  for (int i = 0; i < size1; i++)
    v1[i] = i;
  v2 = v1;
  EXPECT_EQ(size1, v2.GetSize());
}

TEST(TVector, can_assign_vectors_of_different_size)
{
  const int size1 = 4, size2 = 5;
  TVector<int> v1(size1), v2(size2);
  for (int i = 0; i < size1; i++)
    v1[i] = i;
  v2 = v1;
  EXPECT_EQ(0, v2[0]);
  EXPECT_EQ(size1 - 1, v2[size1 - 1]);
}

TEST(TVector, compare_equal_vectors_return_true)
{
  const int size = 4;
  TVector<int> v1(size), v2(size);
  for (int i = 0; i < size; i++)
    v1[i] = i;
  v2 = v1;
  EXPECT_EQ(true, v1 == v2);
}

TEST(TVector, compare_vector_with_itself_return_true)
{
  const int size = 4;
  TVector<int> v(size);
  for (int i = 0; i < size; i++)
    v[i] = i;
  EXPECT_EQ(true, v == v);
}

TEST(TVector, vectors_with_different_size_are_not_equal)
{
  const int size1 = 4, size2 = 5;
  TVector<int> v1(size1), v2(size2);
  EXPECT_NE(v1, v2);
}

TEST(TVector, can_add_scalar_to_vector)
{
  const int size = 4;
  const int val = 3;
  TVector<int> v1(size), v2(size);
  for (int i = 0; i < size; i++)
    v1[i] = i;
  v2 = v1 + val;
  EXPECT_EQ(0 + val, v2[0]);
  EXPECT_EQ(size - 1 + val, v2[size - 1]);
}

TEST(TVector, can_subtract_scalar_from_vector)
{
  const int size = 4;
  const int val = 3;
  TVector<int> v1(size), v2(size);
  for (int i = 0; i < size; i++)
    v1[i] = i;
  v2 = v1 - val;
  EXPECT_EQ(0 - val, v2[0]);
  EXPECT_EQ(size - 1 - val, v2[size - 1]);
}

TEST(TVector, can_multiply_scalar_by_vector)
{
  const int size = 4;
  const int val = 3;
  TVector<int> v1(size), v2(size);
  for (int i = 0; i < size; i++)
    v1[i] = i;
  v2 = v1 * val;
  EXPECT_EQ(0 * val, v2[0]);
  EXPECT_EQ((size - 1) * val, v2[size - 1]);
}

TEST(TVector, can_add_vectors_with_equal_size)
{
  const int size = 4;
  TVector<int> v1(size), v2(size), v3(size);
  for (int i = 0; i < size; i++)
    v1[i] = v2[i] = i;
  v3 = v1 + v2;
  EXPECT_EQ(0, v3[0]);
  EXPECT_EQ(2 * (size - 1), v3[size - 1]);
}

TEST(TVector, cant_add_vectors_with_not_equal_size)
{
  const int size1 = 4, size2 = 5;
  TVector<int> v1(size1), v2(size2);
  for (int i = 0; i < size1; i++)
    v1[i] = i;
  for (int i = 0; i < size2; i++)
    v2[i] = i;
  ASSERT_ANY_THROW(v1 + v2);
}

TEST(TVector, can_subtract_vectors_with_equal_size)
{
  const int size = 4;
  TVector<int> v1(size), v2(size), v3(size);
  for (int i = 0; i < size; i++)
    v1[i] = v2[i] = i;
  v3 = v1 - v2;
  EXPECT_EQ(0, v3[0]);
  EXPECT_EQ(0, v3[size - 1]);
}

TEST(TVector, cant_subtract_vectors_with_not_equal_size)
{
  const int size1 = 4, size2 = 5;
  TVector<int> v1(size1), v2(size2);
  for (int i = 0; i < size1; i++)
    v1[i] = i;
  for (int i = 0; i < size2; i++)
    v2[i] = i;
  ASSERT_ANY_THROW(v1 - v2);
}

TEST(TVector, can_multiply_vectors_with_equal_size)
{
  const int size = 4;
  TVector<int> v1(size), v2(size);
  int res;
  for (int i = 0; i < size; i++)
    v1[i] = v2[i] = 1;
  res = v1 * v2;
  EXPECT_EQ(size, res);
}

TEST(TVector, cant_multiply_vectors_with_not_equal_size)
{
  const int size1 = 4, size2 = 5;
  TVector<int> v1(size1), v2(size2);
  for (int i = 0; i < size1; i++)
    v1[i] = i;
  for (int i = 0; i < size2; i++)
    v2[i] = i;
  ASSERT_ANY_THROW(v1 * v2);
}

