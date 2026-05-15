#pragma once

#include <battery_controller.h>
#include <display.h>
#include <glass_detector.h>
#include <pouring_controller.h>
#include <pump_controller.h>
#include <settings.h>
#include <status_controller.h>
#include <thread.h>
#include <ui_controller.h>
#include <wifi_controller.h>

#include <memory>
#include <mutex>

class MainController : public Thread {
 public:
  MainController();
  ~MainController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  void updateSettingsCallback();
  void startManualPouringCallback();
  void updatePouringController();

  bool m_isSplash;
  UiData m_uiData;
  Settings m_settings;
  StatusController m_statusController;
  BatteryController m_batteryController;
  GlassDetector m_glassDetector;
  Display m_display;
  PumpController m_pumpController;
  WifiController m_wifiController;
  UiController m_uiController;

  std::unique_ptr<PouringController> m_pouringController;
  std::mutex m_mutex;
};
