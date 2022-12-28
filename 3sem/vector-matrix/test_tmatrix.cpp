#include "utmatrix.h"

#include <gtest.h>

TEST(TMatrix, can_create_matrix_with_positive_length)
{
  ASSERT_NO_THROW(TMatrix<int> m(5));
}

TEST(TMatrix, cant_create_too_large_matrix)
{
  ASSERT_ANY_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(TMatrix, throws_when_create_matrix_with_negative_length)
{
  ASSERT_ANY_THROW(TMatrix<int> m(-5));
}

TEST(TMatrix, can_create_copied_matrix)
{
  TMatrix<int> m(5);

  ASSERT_NO_THROW(TMatrix<int> m1(m));
}

TEST(TMatrix, copied_matrix_is_equal_to_source_one)
{
  const int size = 5;
  TMatrix<int> m1(size);
  for (int i = 0; i < size; i++)
    for (int j = i; j < size; j++ )
      m1[i][j] =  i + j;
  TMatrix<int> m2(m1);
  EXPECT_EQ(m1, m2);
}

TEST(TMatrix, copied_matrix_has_its_own_memory)
{
  const int size = 5;
  TMatrix<int> *m1 = new TMatrix<int>(size);
  for (int i = 0; i < size; i++)
    for (int j = i; j < size; j++ )
      (*m1)[i][j] =  i + j;
  TMatrix<int> m2(*m1);
  delete m1;
  EXPECT_EQ(0, m2[0][0]);
  EXPECT_EQ((size - 1) + (size - 1), m2[size - 1][size - 1]);
}

TEST(TMatrix, can_get_size)
{
  TMatrix<int> m(4);

  EXPECT_EQ(4, m.GetSize());
}

TEST(TMatrix, can_set_and_get_element)
{
  TMatrix<int> m(4);
  m[0][0] = 4;

  EXPECT_EQ(4, m[0][0]);
}

TEST(TMatrix, throws_when_set_element_with_negative_index)
{
  TMatrix<int> m(5);
  ASSERT_ANY_THROW(m[-1][0] = 4);
}

TEST(TMatrix, throws_when_set_element_with_too_large_index)
{
  TMatrix<int> m(5);
  ASSERT_ANY_THROW(m[7][0] = 4);
}

TEST(TMatrix, can_assign_matrix_to_itself)
{
  TMatrix<int> m(4);
  ASSERT_NO_THROW(m = m;);
}

TEST(TMatrix, can_assign_matrices_of_equal_size)
{
  const int size = 4;
  TMatrix<int> m1(size), m2(size);
  for (int i = 0; i < size; i++)
    for (int j = i; j < size; j++ )
      m1[i][j] =  i + j;
  m2 = m1;
  EXPECT_EQ(1, m2[0][1]);
  EXPECT_EQ(size - 1 + size - 1, m2[size - 1][size - 1]);
}

TEST(TMatrix, assign_operator_change_matrix_size)
{
  const int size1 = 4, size2 = 5;
  TMatrix<int> m1(size1), m2(size2);
  for (int i = 0; i < size1; i++)
    for (int j = i; j < size1; j++ )
      m1[i][j] =  i + j;
  m2 = m1;
  EXPECT_EQ(size1, m2.GetSize());
}

TEST(TMatrix, can_assign_matrices_of_different_size)
{
  const int size1 = 4, size2 = 5;
  TMatrix<int> m1(size1), m2(size2);
  for (int i = 0; i < size1; i++)
    for (int j = i; j < size1; j++ )
      m1[i][j] =  i + j;
  m2 = m1;
  EXPECT_EQ(1, m2[0][1]);
  EXPECT_EQ(2, m2[1][1]);
  EXPECT_EQ(size1 - 1 + size1 - 1, m2[size1 - 1][size1 - 1]);
}

TEST(TMatrix, compare_equal_matrices_return_true)
{
  const int size = 4;
  TMatrix<int> m1(size), m2(size);
  for (int i = 0; i < size; i++)
    for (int j = i; j < size; j++ )
      m1[i][j] =  i + j;
  m2 = m1;
  EXPECT_TRUE(m1 == m2);
}

TEST(TMatrix, compare_matrix_with_itself_return_true)
{
  const int size = 4;
  TMatrix<int> m(size);
  for (int i = 0; i < size; i++)
    for (int j = i; j < size; j++ )
      m[i][j] =  i + j;
  EXPECT_TRUE(m == m);
}

TEST(TMatrix, matrices_with_different_size_are_not_equal)
{
  const int size1 = 4, size2 = 5;
  TMatrix<int> m1(size1), m2(size2);
  EXPECT_NE(m1, m2);
}

TEST(TMatrix, can_add_matrices_with_equal_size)
{
  const int size = 4;
  TMatrix<int> m1(size), m2(size), m3(size);
  for (int i = 0; i < size; i++)
    for (int j = i; j < size; j++ )
      m1[i][j] = m2[i][j] = i + j;
  m3 = m1 + m2;
  EXPECT_EQ(2, m3[0][1]);
  EXPECT_EQ(4, m3[1][1]);
  EXPECT_EQ(4 * (size - 1), m3[size - 1][size - 1]);
}

TEST(TMatrix, cant_add_matrices_with_not_equal_size)
{
  int i, j;
  const int size1 = 4, size2 = 5;
  TMatrix<int> m1(size1), m2(size2);
  for (i = 0; i < size1; i++)
    for (j = i; j < size1; j++ )
      m1[i][j] = i + j;
  for (i = 0; i < size2; i++)
    for (j = i; j < size2; j++ )
      m2[i][j] = i + j;
  ASSERT_ANY_THROW(m1 + m2);
}

TEST(TMatrix, can_subtract_matrices_with_equal_size)
{
  const int size = 4;
  TMatrix<int> m1(size), m2(size), m3(size);
  for (int i = 0; i < size; i++)
    for (int j = i; j < size; j++ )
      m1[i][j] = m2[i][j] = i + j;
  m3 = m1 - m2;
  EXPECT_EQ(0, m3[0][1]);
  EXPECT_EQ(0, m3[1][1]);
  EXPECT_EQ(0, m3[size - 1][size - 1]);
}

TEST(TMatrix, cant_subtract_matrixes_with_not_equal_size)
{
  int i, j;
  const int size1 = 4, size2 = 5;
  TMatrix<int> m1(size1), m2(size2);
  for (i = 0; i < size1; i++)
    for (j = i; j < size1; j++ )
      m1[i][j] = i + j;
  for (i = 0; i < size2; i++)
    for (j = i; j < size2; j++ )
      m2[i][j] = i + j;
  ASSERT_ANY_THROW(m1 - m2);
}

TEST(TMatrix, can_multiply_matrices_with_equal_size) // TODO
{
  const int size = 3;
  TMatrix<int> m1(size), m2(size), m3(size);
  for (int i = 0; i < size; i++) {
    for (int j = i; j < size; j++ ) {
      m1[i][j] = 1;
      m2[i][j] = 2;
    }
  }
  m3[0][0] = 2; m3[0][1] = 4; m3[0][2] = 6;
                m3[1][1] = 2; m3[1][2] = 4;
                              m3[2][2] = 2;

  std::cout << m1 * m2 << std::endl;
  std::cout << m3 << std::endl;
  EXPECT_EQ(m1 * m2, m3);
}

TEST(TMatrix, can_multiply_ones_matrix) // TODO
{
  const int size = 3;
  TMatrix<int> m1(size), m2(size);
  for (int i = 0; i < size; i++) {
    for (int j = i; j < size; j++ ) {
      m1[i][j] = 1;
      m2[i][j] = 1;
    }
  }
  EXPECT_EQ(m1 * m2, m1);
  EXPECT_EQ(m1 * m2, m2);
}