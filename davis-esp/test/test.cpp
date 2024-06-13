#include <unity.h>
#include "circular-stack.h"
#include "memory.h"
#include "stdio.h"

void setup()
{
}

void tearDown()
{
}

void test_circular_stack_push_pop()
{
    CircularStack stack;

    DavisData *e = stack.pop();
    TEST_ASSERT_NULL(e);
    TEST_ASSERT_EQUAL(0, stack.size());

    e = stack.push();
    e->timestamp = 10;
    e->davisPacket[DAVIS_PACKET_SIZE - 1] = 11;
    TEST_ASSERT_EQUAL(1, stack.size());

    e = stack.push();
    e->timestamp = 20;
    e->davisPacket[DAVIS_PACKET_SIZE - 1] = 21;
    TEST_ASSERT_EQUAL(2, stack.size());

    e = stack.pop();
    TEST_ASSERT_EQUAL(20, e->timestamp);
    TEST_ASSERT_EQUAL(21, e->davisPacket[DAVIS_PACKET_SIZE - 1]);
    TEST_ASSERT_EQUAL(1, stack.size());

    e = stack.pop();
    TEST_ASSERT_EQUAL(10, e->timestamp);
    TEST_ASSERT_EQUAL(11, e->davisPacket[DAVIS_PACKET_SIZE - 1]);
    TEST_ASSERT_EQUAL(0, stack.size());

    e = stack.pop();
    TEST_ASSERT_NULL(e);
    TEST_ASSERT_EQUAL(0, stack.size());
}

void testCircularStackLoop()
{
    CircularStack stack;

    for (int i = 0; i < CIRCULAR_STACK_CAPACITY; ++i)
    {
        DavisData *e = stack.push();
        e->timestamp = 1;
    }

    TEST_ASSERT_EQUAL(CIRCULAR_STACK_CAPACITY, stack.size());
    DavisData *e = stack.push();
    e->timestamp = 2;
    TEST_ASSERT_EQUAL(CIRCULAR_STACK_CAPACITY, stack.size());

    e = stack.pop();
    TEST_ASSERT_EQUAL(2, e->timestamp);
    TEST_ASSERT_EQUAL(CIRCULAR_STACK_CAPACITY - 1, stack.size());

    for (int i = 0; i < CIRCULAR_STACK_CAPACITY - 1; ++i)
    {
        e = stack.pop();
        TEST_ASSERT_EQUAL(1, e->timestamp);
    }
    TEST_ASSERT_EQUAL(0, stack.size());
}

void testCreateHexPacket()
{
    DavisData data = DavisData{
        timestamp : 1718317113,
        usbVoltage : 2932,
        batteryVoltage : 3322,
        davisPacket : {1, 2, 3}
    };
    char expected[300] = {0};
    memset(expected, '0', 299);
    sprintf(expected, "01666b70390000010203");
    expected[14 + 6] = '0';
    sprintf(expected + 14 + 198, "0b740cfa");

    char hex[300];
    createHexPacket(hex, &data);
    TEST_ASSERT_EQUAL_STRING(expected, hex);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_circular_stack_push_pop);
    RUN_TEST(testCircularStackLoop);
    RUN_TEST(testCreateHexPacket);
    UNITY_END();
}
