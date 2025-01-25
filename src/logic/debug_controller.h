#pragma once

#include <logic/logic_controller.h>

class DebugController : public LogicController {
 public:
  DebugController(const Settings& settings, StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController);
  ~DebugController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  void updateDisplay();
};