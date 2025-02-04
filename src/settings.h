#pragma once

#include <helpers.h>

#include <chrono>

class Settings {
 public:
  Settings();

  Mode m_mode;
  int m_distance;
  int m_capacity;
  int m_brightness;
  std::chrono::milliseconds m_glassDetectionDelay;
  std::chrono::milliseconds m_glassDisappearDelay;
  void save();
  void reset();

 private:
  void load(const char* filename);
  void save(const char* filename);
};
