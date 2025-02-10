#pragma once

#include <Arduino.h>

#include <string_view>

#ifdef DEBUG
template <typename... Args>
void log(std::string_view module, std::string_view fmt, Args... args) {
  const auto now = micros();
  Serial.printf("[%5lu.%06lu] [%8s] ", now / 1000000, now % 1000000, module.data());
  Serial.printf(fmt.data(), args...);
  Serial.printf("\n");
}
#else
template <typename... Args>
void log(std::string_view, std::string_view, Args...) {}
#endif