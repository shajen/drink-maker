#pragma once

#include <helpers.h>

#include <chrono>

struct Settings {
  Mode m_mode;
  int m_distance;
  int m_capacity;
  int m_brightness;
  std::chrono::milliseconds m_glassDetectionDelay;
  std::chrono::milliseconds m_glassDisappearDelay;
};
