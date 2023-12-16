#pragma once

#include <helpers.h>

class SettingsController {
 private:
  SettingsController() = delete;
  ~SettingsController() = delete;

 public:
  enum class Type {
    Id = 0,
    Capacity = 1,
    Mode = 2,
    Distance = 3,
    Brightness = 4,
    EditedType = 5,
    GlassDetectionDelay = 6,
    GlassDisappearDelay = 7,
  };

  static void setup();
  static int load(Type type, int defaultValue);
  static void save(Type type, int value);
  static void factoryReset();
};
