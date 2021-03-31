#include "esp_wifi.h"
#include "esp_polled_timeout.h"

bool startWiFi(const char *ssid, const char *password)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("[WiFi] Already connected.");
    return true;
  }
  Serial.println("[WiFi] Connecting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  PolledTimeout timeout(30000);
  while (timeout.running())
  {
    yield();
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.printf("[WiFi] Connected to \"%s\". Local IP is %s. DNS IP is %s.\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), WiFi.dnsIP().toString().c_str());

      WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(1, 1, 1, 1)); 
      delay(10);
      Serial.printf("[WiFi] Changed DNS IP to %s.\n", WiFi.dnsIP().toString().c_str());
      return true;
    }
    else if (WiFi.status() == WL_CONNECT_FAILED)
    {
      Serial.println("[WiFi] Error: Connection failed.");
      return false;
    }
  }
  Serial.printf("[WiFi] Error: Timeout while connecting. Status is %d.\n", WiFi.status());
  return false;
}

void stopWiFi()
{
  WiFi.mode(WIFI_OFF);
}
