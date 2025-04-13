#pragma once

#include <battery_controller.h>
#include <display.h>
#include <glass_detector.h>
#include <led_controller.h>
#include <logger.h>
#include <pump_controller.h>
#include <settings.h>
#include <thread.h>

class PouringController : public Thread {
 public:
  PouringController(
      const Settings& settings, const BatteryController& batteryController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController, int& counter);
  ~PouringController();

  void loop(const std::chrono::milliseconds& now) override;
  void startManualPouring();

 private:
  void loopAuto(const std::chrono::milliseconds& now);
  void loopManual(const std::chrono::milliseconds& now);
  void updateDisplay(const std::chrono::milliseconds remainingTime, float factor);

  const Settings& m_settings;
  const BatteryController& m_batteryController;
  Display& m_display;
  GlassDetector& m_glassDetector;
  LedController& m_ledController;
  PumpController& m_pumpController;

  int& m_counter;
  std::chrono::milliseconds m_startPouringTime;
  std::chrono::milliseconds m_startFullAnimationTime;
  ShortStaticString m_line1;
  ShortStaticString m_line2;
};