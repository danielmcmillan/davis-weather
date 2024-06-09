#ifndef __ESP_WIFI_H__
#define __ESP_WIFI_H__

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#include <WiFiClientSecure.h>
#endif

bool startWiFi(const char *ssid, const char *password);
void stopWiFi();

#endif
