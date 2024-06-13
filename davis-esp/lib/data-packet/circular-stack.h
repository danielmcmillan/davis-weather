#ifndef _circular_stack_h
#define _circular_stack_h

#include <stddef.h>
#include "davis-data.h"

#define CIRCULAR_STACK_CAPACITY 512
#define CIRCULAR_STACK_SIZE (CIRCULAR_STACK_CAPACITY * sizeof(DavisData))

// A stack data structure with fixed capacity that keeps only the most recently pushed items
class CircularStack
{
public:
    CircularStack();
    DavisData *push();
    DavisData *pop();
    size_t size();

private:
    DavisData buffer[CIRCULAR_STACK_CAPACITY];
    size_t nextIndex = 0;
    size_t count = 0;
};

#endif
