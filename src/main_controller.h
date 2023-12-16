#pragma once

#include <OneButton.h>
#include <display.h>
#include <glass_detector.h>
#include <led_controller.h>
#include <logic/logic_controller.h>
#include <pump_controller.h>
#include <status_controller.h>
#include <thread.h>

#include <memory>

class MainController : public Thread {
 public:
  MainController();
  ~MainController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  bool switchDebug();
  bool factoryReset();

  OneButton m_button1;
  OneButton m_button2;

  StatusController m_statusController;
  Display m_display;
  GlassDetector m_glassDetector;
  LedController m_ledController;
  PumpController m_pumpController;

  std::unique_ptr<LogicController> m_logicController;
  bool m_isSplash;
};
