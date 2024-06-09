#include <Arduino.h>
#include "esp_wifi.h"
#include "variables.h"
#include "davis.h"
#include <NTPClient.h>
#include "wifi_mqtt_client.h"

char body[350] = {0};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 7200000);
WifiMqttClient client(AWS_CERT_CA, AWS_IOT_CERT, AWS_IOT_PRIVATE_KEY);

void setup()
{
  Serial.begin(115200);
  davis_setup();
  delay(500);
#ifdef ESP8266
  timeClient.begin();
#endif
}

void loop()
{
  // Get voltages
  // USB voltage: GPIO34
  // Battery voltage: GPIO36
  uint16_t usbVoltage = analogRead(34);
  uint16_t batteryVoltage = analogRead(36);

  if (startWiFi(WIFI_SSID, WIFI_PASSWORD) && client.connect(AWS_IOT_ENDPOINT, 8883, AWS_IOT_THING_NAME))
  {
    timeClient.begin();
    timeClient.update();
    time_t timestamp = timeClient.getEpochTime();
    sprintf(body, "00%08x0000", timestamp);
    size_t prefix_len = strlen(body);
    size_t davis_len = 0;
    if (davis_go(body + prefix_len))
    {
      davis_len = 198;
      // Update first byte to indicate success
      body[1] = '1';
    }
    sprintf(body + prefix_len + davis_len, "%04x%04x", usbVoltage, batteryVoltage);
    Serial.printf("[General] Sending message %s at timestamp %d.\n", body, timestamp);
    client.publish(AWS_IOT_TOPIC, body);

    delay(500);
    client.disconnect();
    stopWiFi();
  }
  Serial.printf("[General] Waiting...\n");
  delay(88000);
}
