#pragma once

#include <config.h>

#include <array>
#include <string>

enum class Mode { None, Auto, Manual };

using ShortStaticString = std::array<char, SHORT_STRING_SIZE>;
using LongStaticString = std::array<char, LONG_STRING_SIZE>;

int voltageToPercentage(const float voltage);

ShortStaticString formatDuration(const std::chrono::milliseconds& now, const bool showMs = true);

char getWifiMode();

char getWifiStatus();

template <typename T>
T mapValue(T value, T inMin, T inMax, T outMin, T outMax) {
  return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}
