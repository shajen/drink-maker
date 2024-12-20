#pragma once

#include <logic/logic_controller.h>

class PouringController : public LogicController {
 public:
  PouringController(StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController);
  ~PouringController();

  void loop(const std::chrono::milliseconds& now) override;
  void onButton1Click() override;
  void onButton2Click() override;
  void onButton1LongClick() override;
  void onButton2LongClick() override;
  void onButton1LongStop() override;
  void onButton2LongStop() override;

 private:
  void loopAuto(const std::chrono::milliseconds& now);
  void loopManual(const std::chrono::milliseconds& now);
  void startManualPouring();
  void updateDisplay(const std::chrono::milliseconds remainingTime, float factor);

  int m_counter;
  std::chrono::milliseconds m_startPouringTime;
  std::chrono::milliseconds m_startFullAnimationTime;
};