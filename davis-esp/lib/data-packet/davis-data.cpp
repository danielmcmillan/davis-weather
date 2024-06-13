#include "davis-data.h"
#include "stdio.h"

char davis_hex_encode_nibble(uint8_t nibble)
{
    nibble &= 0xF;
    return nibble > 9 ? nibble - 10 + 'a' : nibble + '0';
}
/// Encode bytes from the given array as hex characters.
/// `length*2+1` bytes will be written at `result` with a NULL terminator byte.
void davis_hex_encode_array(const uint8_t *data, size_t length, char *result)
{
    for (int i = 0; i < length; ++i)
    {
        result[i * 2] = davis_hex_encode_nibble(data[i] >> 4);
        result[i * 2 + 1] = davis_hex_encode_nibble(data[i]);
    }
    result[length * 2] = 0;
}

void createHexPacket(char *dest, const DavisData *data)
{
    sprintf(dest, "00%08x0000", data->timestamp); // <1 byte success flag, 4 bytes timestamp, 2 bytes reserved>
    size_t prefix_len = 14;
    davis_hex_encode_array(data->davisPacket, DAVIS_PACKET_SIZE, dest + prefix_len);
    size_t davis_len = 198;
    sprintf(dest + prefix_len + davis_len, "%04x%04x", data->usbVoltage, data->batteryVoltage);
    for (int i = 0; i < DAVIS_PACKET_SIZE; ++i)
    {
        if (data->davisPacket[i] != 0)
        {
            dest[1] = '1';
            break;
        }
    }
}
