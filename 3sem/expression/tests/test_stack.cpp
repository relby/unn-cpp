#include "stack.h"

#include <gtest.h>

TEST(Stack, cant_pop_from_empty_stack) {
    Stack<int> s;
    ASSERT_ANY_THROW(s.pop());
}

TEST(Stack, can_create_from_initializer_list) {
    Stack<int> s = {10, 20, 30, 40};
    ASSERT_EQ(s.top(), 40);
    ASSERT_EQ(s.get_size(), 4);
}

TEST(Stack, can_push_to_stack) {
    Stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    ASSERT_EQ(s.top(), 30);
    ASSERT_EQ(s.get_size(), 3);
}

TEST(Stack, can_pop_from_stack) {
    Stack<int> s = {10, 20, 30, 40};
    int x1 = s.pop();
    int x2 = s.pop();

    ASSERT_EQ(x1, 40);
    ASSERT_EQ(x2, 30);
    ASSERT_EQ(s.get_size(), 2);
}

TEST(Stack, is_empty) {
    Stack<int> s = {1, 2, 3};
    s.pop();
    s.pop();
    s.pop();
    ASSERT_EQ(s.get_size(), 0);
    ASSERT_TRUE(s.is_empty());
}
