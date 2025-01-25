#pragma once

#include <display.h>
#include <glass_detector.h>
#include <led_controller.h>
#include <logger.h>
#include <pump_controller.h>
#include <settings.h>
#include <status_controller.h>
#include <thread.h>

#include <chrono>

class LogicController : public Thread {
 public:
  LogicController(const Settings& settings, StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController);
  virtual ~LogicController();

 protected:
  const Settings& m_settings;
  StatusController& m_statusController;
  Display& m_display;
  GlassDetector& m_glassDetector;
  LedController& m_ledController;
  PumpController& m_pumpController;
};
