#include "helpers.h"

#include <Arduino.h>
#include <WiFi.h>
#include <config.h>

#include <cstring>

struct Point {
  float voltage;
  float percent;
};

int voltageToPercentage(const float voltage) {
  static const std::vector<Point> level = {
      {4.20f, 100.f}, {4.10f, 90.f}, {4.00f, 80.f}, {3.90f, 70.f}, {3.80f, 60.f}, {3.70f, 50.f}, {3.60f, 40.f}, {3.50f, 30.f}, {3.40f, 20.f}, {3.30f, 10.f}, {3.20f, 5.f}, {3.00f, 0.f}};

  if (voltage >= level.front().voltage) return 100.f;
  if (voltage <= level.back().voltage) return 0.f;

  for (size_t i = 0; i < level.size() - 1; ++i) {
    if (voltage <= level[i].voltage && voltage >= level[i + 1].voltage) {
      float v1 = level[i].voltage;
      float v2 = level[i + 1].voltage;
      float p1 = level[i].percent;
      float p2 = level[i + 1].percent;

      float t = (voltage - v2) / (v1 - v2);
      return lround(p2 + t * (p1 - p2));
    }
  }

  return 0.f;
}

ShortStaticString formatDuration(const std::chrono::milliseconds& now, const bool showMs) {
  ShortStaticString buffer;
  std::memset(buffer.data(), 0, sizeof(buffer));
  const int milliseconds = now.count() % 1000;
  const int seconds = (now.count() / 1000) % 60;
  const int minutes = (now.count() / 60000) % 60;
  const int hours = (now.count() / 3600000);
  if (showMs) {
    sprintf(buffer.data(), "%02d:%02d:%02d:%03d", hours, minutes, seconds, milliseconds);
  } else {
    sprintf(buffer.data(), "%02d:%02d:%02d", hours, minutes, seconds);
  }
  return buffer;
}

char getWifiMode() {
  switch (WiFi.getMode()) {
    case wifi_mode_t::WIFI_MODE_STA:
      return 'C';
    case wifi_mode_t::WIFI_MODE_AP:
      return 'H';
    case wifi_mode_t::WIFI_MODE_APSTA:
      return 'M';
    default:
      return ' ';
  }
}

char getWifiStatus() { return WiFi.isConnected() ? '+' : '-'; }