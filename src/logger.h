#pragma once

#include <Arduino.h>
#include <helpers.h>

#include <string_view>

#ifdef DEBUG
template <typename... Args>
void log(std::string_view module, std::string_view fmt, Args... args) {
  const auto uptime = formatDuration(std::chrono::milliseconds(millis()));
  Serial.printf("[%s] [%8s] ", uptime.data(), module.data());
  Serial.printf(fmt.data(), args...);
  Serial.printf("\n");
}
#else
template <typename... Args>
void log(std::string_view, std::string_view, Args...) {}
#endif