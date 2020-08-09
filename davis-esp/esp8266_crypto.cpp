#ifdef ESP8266
#include "esp_crypto.h"
#include <Crypto.h>

using namespace experimental::crypto;

void sha256_hash(const uint8_t *payload, size_t payload_length, uint8_t *result)
{
  SHA256::hash(payload, payload_length, result);
}

void sha256_hmac(const uint8_t *payload, size_t payload_length, const uint8_t *key, size_t key_length, uint8_t *result)
{
  SHA256::hmac(payload, payload_length, key, key_length, result, SHA256_LENGTH);
}

#endif
