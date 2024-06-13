#include "circular-stack.h"
#include <string.h>

CircularStack::CircularStack()
{
}

DavisData *CircularStack::push()
{
    int x = CIRCULAR_STACK_SIZE;
    DavisData *item = buffer + nextIndex;
    nextIndex = (nextIndex + 1) % CIRCULAR_STACK_CAPACITY;
    if (count < CIRCULAR_STACK_CAPACITY)
    {
        ++count;
    }
    return item;
}

DavisData *CircularStack::pop()
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
    return buffer + nextIndex;
}

size_t CircularStack::size()
{
    return count;
}
