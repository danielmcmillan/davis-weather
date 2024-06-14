# davis-weather

## davis-esp

Software for ESP8266 or ESP32 for collecting data from Davis Vantage Pro2 weather station and publishing to an MQTT queue.

The MQTT message contains the hex encoding of:

- 1 byte flag: 1 if reading from Davis device was successfully.
- 4 byte timestamp: Unix Epoch timestamp in seconds (unsigned, big-endian)
- 2 byte reserved
- 99 byte Davis LOOP2 packet data
- 2 byte input voltage reading (unsigned, big-endian)
- 2 byte battery voltage reading (unsigned, big-endian)

TODO:

- use sd-card as non-volatile storage to queue data when network is unavailable
- handle missing timestamp, indicate this somehow and use `millis()` to provide some relative time information.
- create template for aws resources
- collect additional data, other than LOOP2 packet

### Wiring

![davis-port](davis-port.png)

- GND pin 16 to ESP ground
- RXD0 pin 5 to ESP TX
- TXD0 pin 6 to ESP RX

The second serial port is used, since first one is used for debugging.

## davis-lib

Javascript library for working with data collected from the Davis weather station.

- extracting raw packet data from a message as published by davis-esp
- CRC validation
- parsing fields from a packet into a JSON object
