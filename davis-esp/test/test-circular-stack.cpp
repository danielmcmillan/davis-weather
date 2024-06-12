#include <unity.h>
#include "circular-stack.h"

void setup()
{
}

void tearDown()
{
}

void test_circular_stack_push_pop()
{
    CircularStack stack;

    char *e = stack.pop();
    TEST_ASSERT_NULL(e);
    TEST_ASSERT_EQUAL(0, stack.size());

    e = stack.push();
    e[0] = 10;
    e[CIRCULAR_STACK_ITEM_SIZE - 1] = 11;
    TEST_ASSERT_EQUAL(1, stack.size());

    e = stack.push();
    e[0] = 20;
    e[CIRCULAR_STACK_ITEM_SIZE - 1] = 21;
    TEST_ASSERT_EQUAL(2, stack.size());

    e = stack.pop();
    TEST_ASSERT_EQUAL(20, e[0]);
    TEST_ASSERT_EQUAL(21, e[CIRCULAR_STACK_ITEM_SIZE - 1]);
    TEST_ASSERT_EQUAL(1, stack.size());

    e = stack.pop();
    TEST_ASSERT_EQUAL(10, e[0]);
    TEST_ASSERT_EQUAL(11, e[CIRCULAR_STACK_ITEM_SIZE - 1]);
    TEST_ASSERT_EQUAL(0, stack.size());

    e = stack.pop();
    TEST_ASSERT_NULL(e);
    TEST_ASSERT_EQUAL(0, stack.size());
}

void test_circular_stack_loop()
{
    CircularStack stack;

    for (int i = 0; i < CIRCULAR_STACK_CAPACITY; ++i)
    {
        char *e = stack.push();
        e[0] = 1;
    }

    TEST_ASSERT_EQUAL(CIRCULAR_STACK_CAPACITY, stack.size());
    char *e = stack.push();
    e[0] = 2;
    TEST_ASSERT_EQUAL(CIRCULAR_STACK_CAPACITY, stack.size());

    e = stack.pop();
    TEST_ASSERT_EQUAL(2, e[0]);
    TEST_ASSERT_EQUAL(CIRCULAR_STACK_CAPACITY - 1, stack.size());

    for (int i = 0; i < CIRCULAR_STACK_CAPACITY - 1; ++i)
    {
        e = stack.pop();
        TEST_ASSERT_EQUAL(1, e[0]);
    }
    TEST_ASSERT_EQUAL(0, stack.size());
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_circular_stack_push_pop);
    RUN_TEST(test_circular_stack_loop);
    UNITY_END();
}
