#include "wifi_mqtt_client.h"
#include "esp_polled_timeout.h"

WifiMqttClient::WifiMqttClient(const char *caCertificate, const char *certificate, const char *privateKey)
    : wifiClient(), mqttClient(256)
{
    // Configure WiFiClientSecure to use the configured credentials
    this->wifiClient.setCACert(caCertificate);
    this->wifiClient.setCertificate(certificate);
    this->wifiClient.setPrivateKey(privateKey);
}

bool WifiMqttClient::connect(const char *endpoint, const int port, const char *clientId)
{
    Serial.println("[MQTT] Connecting...");
    this->mqttClient.begin(endpoint, port, this->wifiClient);

    // Try to connect to the broker
    PolledTimeout timeout(10000);
    while (timeout.running())
    {
        if (mqttClient.connect(clientId))
        {
            break;
        }
        delay(1000);
    }

    if (mqttClient.connected())
    {
        Serial.println("[MQTT] Connected.");
        return true;
    }
    else
    {
        Serial.println("[MQTT] Timeout while connecting.");
        return false;
    }
}

bool WifiMqttClient::disconnect()
{
    return this->mqttClient.disconnect();
}

bool WifiMqttClient::publish(const char *topic, const char *body)
{
    if (this->mqttClient.publish(topic, body))
    {
        return true;
    }
    else
    {
        Serial.println("[MQTT] Failed to publish.");
        return false;
    }
}
