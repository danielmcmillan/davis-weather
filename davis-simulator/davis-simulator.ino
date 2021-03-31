unsigned int state = 0;

void setup()
{
  Serial.begin(19200);
  Serial.printf("Starting Davis simulator...");
  delay(500);
}

char buffer[8];
unsigned int len = 0;

void loop()
{
  while (Serial.available() == 0)
  {
    delay(100);
  }
  // Store next char in buffer
  buffer[len] = (char)Serial.read();
  // When getting \n, clear the buffer. If it was "LSP 2 1" send the packet, otherwise just sent \n\r
  if (buffer[len] == '\n') {
    if (len == 7 && strncmp(buffer, "LPS 2 1", len) == 0) { // TODO check strcmp usage
      // Serial.printf("Sending ACK and 99 byte \"LPS 2 1\" packet");
      Serial.printf("\x06"); // ACK
      Serial.printf("101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101");
      Serial.printf("Finished writing packet");
    } else {
      Serial.printf("Got non-LOOP command, sending \\n\\r");
      Serial.printf("\n\r");
    }
    len = 0;
  } else if (len < 7) {
    // Extend buffer, ignoring last character if buffer is full
    ++len;
  }
}
