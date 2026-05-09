#pragma once

#include <Adafruit_ST7789.h>
#include <battery_controller.h>
#include <helpers.h>
#include <settings.h>
#include <thread.h>

#include <chrono>
#include <string>

class Display : public Thread {
 public:
  enum class State { Splash, Pouring };

  Display(const Settings& settings, const BatteryController& batteryController);
  ~Display();

  void loop(const std::chrono::milliseconds& now) override;
  void setState(const State state);
  void setPouringData(const int counter, const float progress);

 private:
  Adafruit_ST7789 m_display;
  State m_state;
  const Settings& m_settings;
  const BatteryController& m_batteryController;
  ShortStaticString m_capacityData;
  ShortStaticString m_batteryData;
  ShortStaticString m_modeData;
  ShortStaticString m_counterData;
};
