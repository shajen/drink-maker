#pragma once

#include <Arduino.h>
#include <helpers.h>

#include <cstring>
#include <string_view>

#ifdef DEBUG
template <typename... Args>
void log(std::string_view module, std::string_view fmt, Args... args) {
  LongStaticString buffer;
  std::memset(buffer.data(), 0, sizeof(buffer));
  size_t size = 0;
  const auto uptime = formatDuration(std::chrono::milliseconds(millis()));
  size += sprintf(&buffer[size], "[%s] [%8s] ", uptime.data(), module.data());
  size += sprintf(&buffer[size], fmt.data(), args...);
  size += sprintf(&buffer[size], "\n");
  Serial.printf(buffer.data());
  Serial.flush();
}
#else
template <typename... Args>
void log(std::string_view, std::string_view, Args...) {}
#endif
