#ifndef __ESP_POLLED_TIMEOUT_H__
#define __ESP_POLLED_TIMEOUT_H__

#ifdef ESP8266
#include "PolledTimeout.h"
#endif

class PolledTimeout
{
public:
  PolledTimeout(unsigned long duration);
  bool running();

private:
#ifdef ESP8266
  esp8266::polledTimeout::oneShot timeout;
#else
  unsigned long expiry;
#endif
};

#endif
