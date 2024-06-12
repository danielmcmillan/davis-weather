#include "circular-stack.h"
#include <string.h>

CircularStack::CircularStack()
{
}

char *CircularStack::push()
{
    char *item = buffer + nextIndex * CIRCULAR_STACK_ITEM_SIZE;
    nextIndex = (nextIndex + 1) % CIRCULAR_STACK_CAPACITY;
    if (count < CIRCULAR_STACK_CAPACITY)
    {
        ++count;
    }
    return item;
}

char *CircularStack::pop()
{
    if (count == 0)
    {
        return nullptr;
    }
    if (nextIndex == 0)
    {
        nextIndex = CIRCULAR_STACK_CAPACITY - 1;
    }
    else
    {
        --nextIndex;
    }
    --count;
    return buffer + nextIndex * CIRCULAR_STACK_ITEM_SIZE;
}

size_t CircularStack::size()
{
    return count;
}
