#include <Arduino.h>
#include "wifi-manager.h"
#include "settings.h"
#include "davis.h"
#include <NTPClient.h>
#include "mqtt-client.h"
#include "logging.h"
#include "circular-stack.h"
#include "davis-data.h"

WiFiManager wifi(WIFI_SSID, WIFI_PASSWORD);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 7200000);
CircularStack pendingPackets;
MqttClient mqtt(MQTT_BROKER_ENDPOINT, MQTT_BROKER_PORT, MQTT_CLIENT_ID, MQTT_BROKER_CA_CERT, MQTT_CLIENT_CERT, MQTT_CLIENT_KEY);
char buffer[HEX_PACKET_SIZE + 1];

unsigned long lastStatusCheckMillis = 0;
unsigned long lastReadingMillis = 0;
uint16_t usbVoltage;

#define UPDATE_INTERVAL (88 * 1000)
#define READ_INTERVAL_DISCONNECTED (10 * 60 * 1000)

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
  bool doStatusChecks = lastStatusCheckMillis == 0 || now - lastStatusCheckMillis >= UPDATE_INTERVAL;

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
    LOG_INFO("[General] disconnecting WiFi since power is disconnected: %d", usbVoltage);
    mqtt.loop(false);
    wifi.loop(false);
    connected = false;
  }

  if ((doStatusChecks && connected) || lastReadingMillis == 0 || now - lastReadingMillis >= READ_INTERVAL_DISCONNECTED)
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

  DavisData *item = pendingPackets.push();
  item->timestamp = timeClient.getEpochTime();
  item->usbVoltage = usbVoltage;
  item->batteryVoltage = analogRead(36); // 3322 = 4.1v
  if (!davis_go(item->davisPacket))
  {
    LOG_ERROR("[General] failed to read from Davis");
  }

  // Send pending packets
  while (connected && pendingPackets.size() > 0)
  {
    DavisData *item = pendingPackets.pop();
    createHexPacket(buffer, item);
    Serial.printf("[General] sending message %s (timestamp %d, %d remaining)\n", buffer, item->timestamp, pendingPackets.size());
    connected = mqtt.publish(MQTT_TOPIC, buffer, HEX_PACKET_SIZE);
  }
  Serial.printf("[General] waiting...\n");
}
