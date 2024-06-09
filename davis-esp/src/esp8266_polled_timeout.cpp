#ifdef ESP8266
#include "esp_polled_timeout.h"

PolledTimeout::PolledTimeout(unsigned long duration) : timeout(duration)
{
}

bool PolledTimeout::running()
{
  return !this->timeout;
}

#endif
