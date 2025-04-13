#pragma once

#include <config.h>

#include <array>
#include <string>

enum class Mode { None, Auto, Manual };

using ShortStaticString = std::array<char, SHORT_STRING_SIZE>;
using LongStaticString = std::array<char, LONG_STRING_SIZE>;

int voltageToPercentage(const float voltage);