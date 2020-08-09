#ifndef ESP8266
#include <Arduino.h>
#include "esp_polled_timeout.h"

PolledTimeout::PolledTimeout(unsigned long duration) : expiry(millis() + duration)
{
}

bool PolledTimeout::running()
{
  return millis() < this->expiry;
}

#endif
