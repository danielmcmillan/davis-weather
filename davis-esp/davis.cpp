#include "davis.h"
#include <Arduino.h>
#include "esp_polled_timeout.h"

#define DAVIS_RXD 16 // RX2
#define DAVIS_TXD 17 // TX2

#define DAVIS_SERIAL Serial2
// Time in milliseconds to wait for initial wakeup
#define DAVIS_WAKEUP_TIMEOUT 2000
// Time in milliseconds to wait after talking to Davis
#define DAVIS_READ_TIMEOUT 500
// Time between each check for data
#define DAVIS_READ_INTERVAL 100

#define ACK 0x06

void davis_setup()
{
  DAVIS_SERIAL.begin(19200, SERIAL_8N1, DAVIS_RXD, DAVIS_TXD);
  // pinMode(2, OUTPUT);
}

/// Wait up to DAVIS_READ_TIMEOUT for data to become available.
bool davis_wait_for_data()
{
  if (DAVIS_SERIAL.available() == 0)
  {
    // Wait until data is available
    PolledTimeout timeout(DAVIS_READ_TIMEOUT);
    while (DAVIS_SERIAL.available() == 0 && timeout.running())
    {
      delay(DAVIS_READ_INTERVAL);
    }
  }
  return DAVIS_SERIAL.available() > 0;
}

/// Read up to the specified number of bytes and store to the buffer.
/// Returns the number of bytes read.
size_t davis_read(uint8_t *buffer, size_t length)
{
  size_t read = 0;
  for (; read < length; ++read)
  {
    if (davis_wait_for_data())
    {
      *buffer = (uint8_t)DAVIS_SERIAL.read();
      ++buffer;
    }
    else
    {
      break;
    }
  }
  return read;
}

/// Consume data from the DAVIS_SERIAL input buffer until given data sequence is reached.
/// Returns whether the desired character sequence was found and consumed.
bool davis_consume_to(const uint8_t *sequence, size_t length)
{
  size_t sequence_index = 0;
  PolledTimeout timeout(DAVIS_READ_TIMEOUT * length);
  while (timeout.running())
  {
    int data = DAVIS_SERIAL.read();
    if (data < 0)
    {
      // No more data, sequence not found
      return false;
    }
    else if (data == sequence[sequence_index])
    {
      // Next char from sequence sequence found
      ++sequence_index;
      if (sequence_index == length)
      {
        // Entire sequence found
        return true;
      }
    }
    else
    {
      // Unexpected character, reset progress within sequence
      sequence_index = 0;
    }

    Serial.print("[DAVIS] Consumed: \"");
    Serial.print(data, DEC);
    Serial.println("\".");
  }
  return false; // Timed out waiting for expected sequence
}

bool davis_wake()
{
  // Try waking procedure up to 3 times
  for (int i = 0; i < 3; ++i)
  {
    Serial.println("[DAVIS] Attempting wakeup");
    DAVIS_SERIAL.write("\n");
    // Wait for "\n\r" response for up to 1.5 seconds
    PolledTimeout timeout(DAVIS_WAKEUP_TIMEOUT);
    while (timeout.running())
    {
      delay(DAVIS_READ_INTERVAL);
      if (DAVIS_SERIAL.available() > 0)
      {
        // Got some data, try to find \n\r
        if (davis_consume_to((uint8_t *)"\n\r", 2))
        {
          Serial.println("[DAVIS] Wakeup completed successfully.");
          return true;
        }
        else
        {
          Serial.println("[DAVIS] Unexpected data received after wakeup.");
        }
      }
    }
  }
  Serial.println("[DAVIS] Timeout while waking up Davis.");
  return false;
}

bool davis_loop_packet(uint8_t *loop_packet)
{
  // TODO loop type
  const uint8_t ack_sequence[1] = {ACK};
  DAVIS_SERIAL.write("LPS 2 1\n");
  if (davis_wait_for_data() && davis_consume_to(ack_sequence, 1))
  {
    size_t read = davis_read(loop_packet, 99);
    return read == 99; // TODO Test this with real device.
  }
  else
  {
    return false;
  }
}

// TEMP for testing
char davis_hex_encode_nibble(uint8_t nibble)
{
  nibble &= 0xF;
  return nibble > 9 ? nibble - 10 + 'a' : nibble + '0';
}
/// Encode bytes from the given array as hex characters.
/// `length*2+1` bytes will be written at `result` with a NULL terminator byte.
void davis_hex_encode_array(uint8_t *data, size_t length, char *result)
{
  for (int i = 0; i < length; ++i)
  {
    result[i * 2] = davis_hex_encode_nibble(data[i] >> 4);
    result[i * 2 + 1] = davis_hex_encode_nibble(data[i]);
  }
  result[length * 2] = 0;
}

boolean davis_go(char *hex)
{
  uint8_t loop_packet[99] = {0};
  if (davis_wake() && davis_loop_packet(loop_packet))
  {
    davis_hex_encode_array(loop_packet, 99, hex);
    return true;
  }
  else
  {
    return false;
  }
}

// void flash_lamps(bool woken)
// {
//   int interval = woken ? 300 : 1000;
//   while (true)
//   {
//     DAVIS_SERIAL.write("LAMPS 1\n");
//     digitalWrite(2, HIGH);
//     davis_consume_to("\n\rOK\n\r");
//     delay(interval);
//     DAVIS_SERIAL.write("LAMPS 0\n");
//     digitalWrite(2, LOW);
//     davis_consume_to("\n\rOK\n\r");
//     delay(interval);
//   }
// }

/// Clear incoming data currently in the DAVIS_SERIAL buffer
// void davis_clear()
// {
//   for (int i = DAVIS_SERIAL.available(); i > 0; --i)
//   {
//     DAVIS_SERIAL.read();
//   }
// }
