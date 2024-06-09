#include "wifi-manager.h"
#include "logging.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#include <WiFiClientSecure.h>
#endif

WiFiManager::WiFiManager(const char *ssid, const char *password) : ssid(ssid), password(password)
{
}

void WiFiManager::setup(const char *hostname)
{
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname);
}

bool WiFiManager::loop(bool connect)
{
    if (!connect)
    {
        WiFi.mode(WIFI_OFF);
        return false;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        if (WiFi.localIP() == INADDR_NONE)
        {
            return false;
        }
        return true;
    }

    LOG_INFO("[WiFi] connecting");
    WiFi.begin(ssid, password);

    for (int i = 0; i < 20; ++i)
    {
        delay(500);
        if (WiFi.status() == WL_CONNECTED)
        {
            LOG_INFO("[WiFi] connected successfully");
            return true;
        }
        else if (WiFi.status() == WL_CONNECT_FAILED)
        {
            LOG_ERROR("[WiFi] connection failed");
            return false;
        }
    }
    LOG_ERROR("[WiFi] connection timed out");
    return false;

    // Serial.printf("[WiFi] Connected to \"%s\". Local IP is %s. DNS IP is %s.\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), WiFi.dnsIP().toString().c_str());
    // WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(1, 1, 1, 1));
    // delay(10);
    // Serial.printf("[WiFi] Changed DNS IP to %s.\n", WiFi.dnsIP().toString().c_str());
}
