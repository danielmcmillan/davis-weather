#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

class WiFiManager
{
public:
    WiFiManager(const char *ssid, const char *password);
    void setup(const char *hostname);
    /**
     * Maintain WiFi connection. Returns whether connnection is currently active.
     */
    bool loop(bool connect);

private:
    const char *ssid;
    const char *password;
};

#endif
