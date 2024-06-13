#ifndef __DAVIS_H__
#define __DAVIS_H__

#include <stdint.h>

void davis_setup();
bool davis_go(uint8_t *hex);

#endif
