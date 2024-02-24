//
// Expanded tests for int_stack operations.
//

#include <gtest/gtest.h>
#include "int_stack.hh"

TEST(IntStackTests, Initialization) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    ASSERT_EQ(int_stack_size(&stack), 0);
    ASSERT_EQ(int_stack_capacity(&stack), 10);
}

TEST(IntStackTests, PushToCapacityAndOverflow) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(int_stack_push(&stack, i));
    }
    ASSERT_FALSE(int_stack_push(&stack, 11)); // Test overflow
}

TEST(IntStackTests, PushToCapacityPopUntilUnderflow) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(int_stack_push(&stack, i));
    }
    int value;
    for (int i = 9; i >= 0; i--) {
        ASSERT_TRUE(int_stack_pop(&stack, &value));
        ASSERT_EQ(value, i);
    }
    ASSERT_FALSE(int_stack_pop(&stack, &value)); // Test underflow
}

TEST(IntStackTests, TopValue) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 5);
    int value;
    ASSERT_TRUE(int_stack_top(&stack, &value));
    ASSERT_EQ(value, 5);
}

TEST(IntStackTests, DuplicateTop) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 5);
    ASSERT_TRUE(int_stack_dup(&stack));
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 5);
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 5);
}

TEST(IntStackTests, SwapTopTwo) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 1);
    int_stack_push(&stack, 2);
    ASSERT_TRUE(int_stack_swap(&stack));
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 1);
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 2);
}

TEST(IntStackTests, CopySecondItemToTop) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 1);
    int_stack_push(&stack, 2);
    ASSERT_TRUE(int_stack_over(&stack));
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 1);
}

TEST(IntStackTests, RotateTopThree) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 1); // Bottom of the three
    int_stack_push(&stack, 2); // Middle
    int_stack_push(&stack, 3); // Top

    ASSERT_TRUE(int_stack_rot(&stack));

    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 1); // Now on top after rotation

    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 3); // Now in the middle

    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 2); // Now at the bottom
}

TEST(IntStackTests, DropTop) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 1);
    int_stack_push(&stack, 2);
    ASSERT_TRUE(int_stack_drop(&stack));
    ASSERT_EQ(int_stack_size(&stack), 1); // Size should decrease by 1
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 1); // Verify that 2 was dropped
}

TEST(IntStackTests, SwapTopTwoPairs) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 1);
    int_stack_push(&stack, 2);
    int_stack_push(&stack, 3);
    int_stack_push(&stack, 4);
    ASSERT_TRUE(int_stack_2swap(&stack));
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 2); // After swap, 2 is top
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 1); // Then 1
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 4); // Then 4 (swapped)
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 3); // Then 3 (swapped)
}

TEST(IntStackTests, DuplicateTopTwo) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 1);
    int_stack_push(&stack, 2);
    ASSERT_TRUE(int_stack_2dup(&stack));
    ASSERT_EQ(int_stack_size(&stack), 4); // Size should double
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 2); // Duplicated values on top
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 1); // Duplicated values on top
}

TEST(IntStackTests, TwoOverOperation) {
    int_stack_t stack;
    int_stack_init(&stack, 10); // Correct initialization with capacity

    // Push some values onto the stack
    int_stack_push(&stack, 3); // Bottom
    int_stack_push(&stack, 5);
    int_stack_push(&stack, 8);
    int_stack_push(&stack, 1); // Top

    // Perform the 2OVER operation
    ASSERT_TRUE(int_stack_2over(&stack)); // Verify that 2OVER operation succeeds

    // After the 2OVER operation, the stack should now be: 3, 5, 8, 1, 8 (from bottom to top)

    // Verify the stack size has increased by 1, indicating a new item was added
    ASSERT_EQ(int_stack_size(&stack), 5);

    // Verify the top of the stack is now the duplicated value (8, which was the second item from the top)
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 8); // Top value after 2OVER

    // Verify the rest of the stack is as expected
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 1); // Then the value that was at the top
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 8); // Then the original second-to-top value, now in the middle
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 5); // Then the next value
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 3); // Finally, the original bottom value
}




TEST(IntStackTests, DropTopTwo) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 1);
    int_stack_push(&stack, 2);
    int_stack_push(&stack, 3);
    ASSERT_TRUE(int_stack_2drop(&stack));
    ASSERT_EQ(int_stack_size(&stack), 1); // Size decreases by 2
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 1); // Verify remaining value
}

TEST(IntStackTests, AddTopTwoValues) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int_stack_push(&stack, 1);
    int_stack_push(&stack, 2);
    ASSERT_TRUE(int_stack_add(&stack));
    ASSERT_EQ(int_stack_size(&stack), 1); // Size decreases by 1 after add
    int value;
    int_stack_pop(&stack, &value);
    ASSERT_EQ(value, 3); // Verify addition result
}



// Edge Case Tests
TEST(IntStackTests, EmptyStackOperations) {
    int_stack_t stack;
    int_stack_init(&stack, 10);
    int value;
    ASSERT_FALSE(int_stack_pop(&stack, &value)); // Pop from empty
    ASSERT_FALSE(int_stack_top(&stack, &value)); // Top from empty
    ASSERT_FALSE(int_stack_dup(&stack)); // Dup on empty
    ASSERT_FALSE(int_stack_swap(&stack)); // Swap on empty
}

TEST(IntStackTests, CapacityLimits) {
    int_stack_t stack;
    int_stack_init(&stack, 2);
    ASSERT_TRUE(int_stack_push(&stack, 1));
    ASSERT_TRUE(int_stack_push(&stack, 2));
    ASSERT_FALSE(int_stack_push(&stack, 3)); // Push beyond capacity
    int value;
    ASSERT_TRUE(int_stack_pop(&stack, &value)); // Pop to make space
    ASSERT_TRUE(int_stack_push(&stack, 3)); // Push should succeed now
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
