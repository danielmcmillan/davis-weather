#ifndef __ESP_CRYPTO_H__
#define __ESP_CRYPTO_H__
#include <Arduino.h>

// Length in bytes of SHA256 hash
#define SHA256_LENGTH 32

void sha256_hash(const uint8_t *payload, size_t payload_length, uint8_t *result);
void sha256_hmac(const uint8_t *payload, size_t payload_length, const uint8_t *key, size_t key_length, uint8_t *result);

#endif
