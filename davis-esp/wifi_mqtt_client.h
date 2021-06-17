#ifndef __WIFI_MQTT_CLIENT_H__
#define __WIFI_MQTT_CLIENT_H__

#include <WiFiClientSecure.h>
#include <MQTTClient.h>

class WifiMqttClient
{
    WiFiClientSecure wifiClient;
    MQTTClient mqttClient;

public:
    WifiMqttClient(const char *caCertificate, const char *certificate, const char *privateKey);
    /// Start client and connect to the MQTT broker.
    /// @returns Whether the connection was successful.
    bool connect(const char *endpoint, const int port, const char *clientId);
    /// Disconnec from the MQTT broker.
    /// @returns Whether the disconnect was successful.
    bool disconnect();
    /// Send a message.
    /// Returns whether it was sent successfully.
    bool publish(const char *topic, const char *body);
};

#endif
