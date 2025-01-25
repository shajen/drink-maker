#pragma once

#include <logic/logic_controller.h>

class SplashController : public LogicController {
 public:
  SplashController(const Settings& settings, StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController);
  ~SplashController();

  void loop(const std::chrono::milliseconds& now) override;
};