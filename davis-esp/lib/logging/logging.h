#ifndef _LOGGING_H
#define _LOGGING_H

#ifndef DISABLE_LOGGING
#define LOG_INFO(msg, ...) Serial.printf("INFO " msg "\n" __VA_OPT__(, ) __VA_ARGS__)
#define LOG_ERROR(msg, ...) Serial.printf("ERROR " msg "\n" __VA_OPT__(, ) __VA_ARGS__)
#else
#define LOG_INFO(msg, ...)
#define LOG_ERROR(msg, ...)
#endif

#endif
