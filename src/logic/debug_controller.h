#pragma once

#include <logic/logic_controller.h>

class DebugController : public LogicController {
 public:
  DebugController(StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController);
  ~DebugController();

  void loop(const std::chrono::milliseconds& now) override;
  void onButton1Click() override;
  void onButton2Click() override;
  void onButton1LongClick() override;
  void onButton2LongClick() override;

 private:
  enum class Type {
    Mode = 0,
    Capacity = 1,
    Distance = 2,
    Brightness = 3,
    GlassDetectionDelay = 4,
    GlassDisappearDelay = 5,
    CurrentDistance = 6,
    Max = 7,
  };

  void switchMode();
  void updateDisplay();

  Type m_editedType;
};