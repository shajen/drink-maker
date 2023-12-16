#pragma once

#include <display.h>
#include <glass_detector.h>
#include <led_controller.h>
#include <logger.h>
#include <pump_controller.h>
#include <settings_controller.h>
#include <status_controller.h>
#include <thread.h>

#include <chrono>

class LogicController : public Thread {
 public:
  LogicController(StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController);
  virtual ~LogicController();

  virtual void onButton1Click();
  virtual void onButton2Click();
  virtual void onButton1LongClick();
  virtual void onButton2LongClick();

 protected:
  int m_capacity;
  int m_distance;
  int m_brightness;
  Mode m_mode;
  std::chrono::milliseconds m_glassDetectionDelay;
  std::chrono::milliseconds m_glassDisappearDelay;

  StatusController& m_statusController;
  Display& m_display;
  GlassDetector& m_glassDetector;
  LedController& m_ledController;
  PumpController& m_pumpController;
};