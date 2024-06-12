#ifndef _CIRCULAR_STACK_H
#define _CIRCULAR_STACK_H

#include <stddef.h>

#define CIRCULAR_STACK_ITEM_SIZE (14 + 198 + 8)
#define CIRCULAR_STACK_CAPACITY 256

// A stack data structure with fixed capacity that keeps only the most recently pushed items
class CircularStack
{
public:
    CircularStack();
    char *push();
    char *pop();
    size_t size();

private:
    char buffer[CIRCULAR_STACK_ITEM_SIZE * CIRCULAR_STACK_CAPACITY];
    size_t nextIndex = 0;
    size_t count = 0;
};

#endif
