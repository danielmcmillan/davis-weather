#include <Arduino.h>
#include "wifi-manager.h"
#include "settings.h"
#include "davis.h"
#include <NTPClient.h>
#include "mqtt-client.h"
#include "logging.h"

char body[350] = {0};
WiFiManager wifi(WIFI_SSID, WIFI_PASSWORD);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 7200000);
MqttClient mqtt(MQTT_BROKER_ENDPOINT, MQTT_BROKER_PORT, MQTT_CLIENT_ID, MQTT_BROKER_CA_CERT, MQTT_CLIENT_CERT, MQTT_CLIENT_KEY);

unsigned long lastStatusCheckMillis = 0;
unsigned long lastReadingMillis = 0;
uint16_t usbVoltage;

#define UPDATE_INTERVAL (88 * 1000)
#define READ_INTERVAL_DISCONNECTED (15 * 60 * 1000)

void setup()
{
  Serial.begin(115200);
  davis_setup();
  wifi.setup("davis-esp");
  delay(500);
#ifdef ESP8266
  timeClient.begin();
#endif
}

void loop()
{
  unsigned long now = millis();
  bool doStatusChecks = now - lastStatusCheckMillis >= UPDATE_INTERVAL;

  if (doStatusChecks)
  {
    lastStatusCheckMillis = now;
    usbVoltage = analogRead(34); // 2932 = 5.0v
  }
  bool isCharging = usbVoltage > 2345; // ~4v

  bool connected = false;
  if (isCharging)
  {
    connected = wifi.loop(true) && mqtt.loop(true);
    if (connected)
    {
      timeClient.begin();
      timeClient.update();
    }
    else
    {
      // TODO if failed many times, then restart ESP
    }
  }
  else if (doStatusChecks)
  {
    // TODO low power sleep when not charging
    LOG_INFO("[General] disconnecting WiFi since power is disconnected");
    mqtt.loop(false);
    wifi.loop(false);
    connected = false;
  }

  if ((doStatusChecks && connected) || now - lastReadingMillis >= READ_INTERVAL_DISCONNECTED)
  {
    lastReadingMillis = now;
    if (!timeClient.isTimeSet())
    {
      LOG_ERROR("[General] aborting since no time is available");
      return;
    }
  }
  else
  {
    return;
  }

  // TODO always write to an in-memory queue, flush to mqtt when connected
  uint16_t batteryVoltage = analogRead(36); // 3322 = 4.1v
  if (connected)
  {
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
    Serial.printf("[General] sending message %s at timestamp %d.\n", body, timestamp);
    mqtt.publish(MQTT_TOPIC, body, prefix_len + davis_len + 4);
  }
  Serial.printf("[General] waiting...\n");
}
