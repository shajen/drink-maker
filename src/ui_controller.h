#pragma once

#include <ESPUI.h>
#include <Ticker.h>
#include <settings.h>
#include <thread.h>

#include <chrono>
#include <functional>
#include <vector>

class UiController : public Thread {
 public:
  UiController(Settings& settings, std::function<void()> updateSettingsCallback, std::function<int()> getDistanceCallback, std::function<void()> manualPourCallback);
  ~UiController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  void updateManualPourButton();
  void saveSettings();
  void manualPour(Control* sender, int type);
  void resetSettings(Control* sender, int type);
  void reboot(Control* sender, int type);

  Settings& m_settings;
  std::function<void()> m_updateSettingsCallback;
  std::function<int()> m_getDistanceCallback;
  std::function<void()> m_manualPourCallback;
  std::vector<std::function<void()>> m_readSettingsCallbacks;
  Ticker m_rebootTimer;
  bool m_isDebugTab;
  std::chrono::milliseconds m_lastPrintStatusTime;

  uint16_t m_manualPourButton;
  uint16_t m_uptimeControl;
  uint16_t m_heapControl;
  uint16_t m_distance;
};
