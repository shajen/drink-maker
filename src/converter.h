#pragma once

#include <Arduino.h>
#include <helpers.h>

#include <chrono>

template <typename T>
T fromString(const String& value);

template <>
int fromString<int>(const String& value) {
  return value.toInt();
}

template <>
std::chrono::milliseconds fromString<std::chrono::milliseconds>(const String& value) {
  return std::chrono::milliseconds(fromString<int>(value));
}

template <>
Mode fromString<Mode>(const String& value) {
  return static_cast<Mode>(fromString<int>(value));
}

template <>
std::string fromString<std::string>(const String& value) {
  return std::string(value.c_str());
}

template <typename T>
String toString(const T& value);

template <>
String toString<int>(const int& value) {
  return String(value);
}

template <>
String toString<std::chrono::milliseconds>(const std::chrono::milliseconds& value) {
  return String(value.count());
}

template <>
String toString<Mode>(const Mode& value) {
  return String(static_cast<int>(value));
}

template <>
String toString<std::string>(const std::string& value) {
  return String(value.c_str());
}
