#ifndef _davis_data_h
#define _davis_data_h

#include <stdint.h>

#define DAVIS_PACKET_SIZE 99
#define HEX_PACKET_SIZE (2 * (7 + DAVIS_PACKET_SIZE + 4))

struct DavisData
{
    uint32_t timestamp;
    uint16_t usbVoltage;
    uint16_t batteryVoltage;
    uint8_t davisPacket[DAVIS_PACKET_SIZE];
};

void createHexPacket(char *dest, const DavisData *data);

#endif
