#include <pgmspace.h>

/**
 * Network connection details.
 */
const char *WIFI_SSID = "ssid";
const char *WIFI_PASSWORD = "password";

const char *MQTT_BROKER_ENDPOINT = "";
const int MQTT_BROKER_PORT = 8883;
const char *MQTT_CLIENT_ID = "";
const char *MQTT_TOPIC = "";

// Amazon Root CA 1
static const char *MQTT_BROKER_CA_CERT PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char *MQTT_CLIENT_CERT PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char *MQTT_CLIENT_KEY PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)KEY";
