#pragma once

#include <ESPUI.h>
#include <Ticker.h>
#include <battery_controller.h>
#include <glass_detector.h>
#include <settings.h>
#include <status_controller.h>
#include <thread.h>

#include <chrono>
#include <functional>
#include <vector>

struct UiData {
  UiData();

  int m_pourCount;
  int m_distanceErrorCount;
};

class UiController : public Thread {
 public:
  UiController(
      const BatteryController& batteryController,
      const StatusController& statusController,
      const GlassDetector& glassDetector,
      const UiData& uiData,
      Settings& settings,
      std::function<void()> updateSettingsCallback,
      std::function<void()> manualPourCallback);
  ~UiController();

  void loop(const std::chrono::milliseconds& now) override;
  bool isActive() const;

 private:
  void updateManualPourButton();
  void saveSettings();
  void manualPour(Control* sender, int type);
  void resetSettings(Control* sender, int type);
  void reboot(Control* sender, int type);

  const BatteryController& m_batteryController;
  const StatusController& m_statusController;
  const GlassDetector& m_glassDetector;
  const UiData& m_uiData;
  Settings& m_settings;
  std::function<void()> m_updateSettingsCallback;
  std::function<void()> m_manualPourCallback;
  std::vector<std::function<void()>> m_readSettingsCallbacks;
  Ticker m_rebootTimer;
  bool m_isDebugTab;
  std::chrono::milliseconds m_lastPrintStatusTime;

  uint16_t m_manualPourButton;
  uint16_t m_pourCountControl;
  uint16_t m_uptimeControl;
  uint16_t m_heapControl;
  uint16_t m_distanceErrorCountControl;
  uint16_t m_distanceControl;
  uint16_t m_fpsControl;
  uint16_t m_batteryVoltageControl;
  uint16_t m_batteryPercentageControl;
};
