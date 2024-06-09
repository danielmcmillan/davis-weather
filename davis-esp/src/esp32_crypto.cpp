#ifndef ESP8266
#include "esp_crypto.h"
#include "mbedtls/md.h"

void sha256_hash(const uint8_t *payload, size_t payload_length, uint8_t *result)
{
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *)payload, payload_length);
  mbedtls_md_finish(&ctx, result);
  mbedtls_md_free(&ctx);
}

void sha256_hmac(const uint8_t *payload, size_t payload_length, const uint8_t *key, size_t key_length, uint8_t *result)
{
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char *)key, key_length);
  mbedtls_md_hmac_update(&ctx, (const unsigned char *)payload, payload_length);
  mbedtls_md_hmac_finish(&ctx, result);
  mbedtls_md_free(&ctx);
}

#endif
