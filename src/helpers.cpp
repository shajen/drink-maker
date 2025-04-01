#include "helpers.h"

#include <Arduino.h>
#include <config.h>

struct Point {
  float voltage;
  float percent;
};

float voltageToPercentage(const float voltage) {
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
      return p2 + t * (p1 - p2);
    }
  }

  return 0.f;
}