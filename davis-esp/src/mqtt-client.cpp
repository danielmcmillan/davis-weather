#include "mqtt-client.h"
#include "logging.h"

MqttClient::MqttClient(const char *endpoint, int port, const char *clientId, const char *caCertificate, const char *certificate, const char *privateKey)
    : wifiClient(), mqttClient(512), endpoint(endpoint), port(port), clientId(clientId)
{
    // Configure WiFiClientSecure to use the configured credentials
    this->wifiClient.setCACert(caCertificate);
    this->wifiClient.setCertificate(certificate);
    this->wifiClient.setPrivateKey(privateKey);

    mqttClient.begin(endpoint, port, this->wifiClient);
    mqttClient.setOptions(
        10,    // keepAlive
        false, // cleanSession, use persistent sessions
        10000  // timeout
    );
}

bool MqttClient::loop(bool connect)
{
    if (!connect)
    {
        mqttClient.disconnect();
        return false;
    }
    bool connected = mqttClient.loop();

    if (!connected)
    {
        LOG_INFO("[MQTT] connecting");
        for (int i = 0; i < 10; ++i)
        {
            if (mqttClient.connect(clientId))
            {
                connected = true;
                break;
            }
            delay(500);
        }
        if (connected)
        {
            if (mqttClient.sessionPresent())
            {
                LOG_INFO("[MQTT] connected with existing session");
            }
            else
            {
                LOG_INFO("[MQTT] connected with new session");
            }
        }
        else
        {
            LOG_ERROR("[MQTT] connection failed: %d", mqttClient.lastError());
        }
    }

    return connected;
}

bool MqttClient::publish(const char *topic, const char *payload, int length, bool ignoreError) const
{
    if (this->mqttClient.publish(topic, payload, length, false, 1))
    {
        return true;
    }
    else
    {
        if (!ignoreError)
        {
            LOG_ERROR("[MQTT] publish failed: %d", mqttClient.lastError());
        }
        return false;
    }
}
