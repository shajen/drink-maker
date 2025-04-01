#pragma once

#include <battery_controller.h>
#include <display.h>
#include <glass_detector.h>
#include <led_controller.h>
#include <pump_controller.h>
#include <settings.h>
#include <status_controller.h>
#include <thread.h>
#include <ui_controller.h>
#include <wifi_controller.h>

#include <memory>

class MainController : public Thread {
 public:
  MainController();
  ~MainController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  void updateSettingsCallback();
  void startManualPouringCallback();
  void updateLogicController();

  bool m_isSplash;
  int m_counter;
  Settings m_settings;
  StatusController m_statusController;
  BatteryController m_batteryController;
  Display m_display;
  GlassDetector m_glassDetector;
  LedController m_ledController;
  PumpController m_pumpController;
  WifiController m_wifiController;
  UiController m_uiController;

  std::unique_ptr<Thread> m_logicController;
};
