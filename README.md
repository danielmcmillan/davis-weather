# davis-weather

## davis-esp

Software for ESP8266 or ESP32 for collecting data from Davis Vantage Pro2 weather station and publishing to an MQTT queue.

TODO:

- handle missing network by queueing data to be stored. Timestamp may be unknown, indicate this somehow and use `millis()` to provide some relative time information.
- use sd-card as non-volatile storage for queue
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
