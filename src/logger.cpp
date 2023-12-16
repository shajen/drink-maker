#include <Arduino.h>
#include <logger.h>

void log(const char* module, const char* message) {
  const auto now = millis();
  printf("[%5ld.%03ld] [%s] %s\n", now / 1000, now % 1000, module, message);
}