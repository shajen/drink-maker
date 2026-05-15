#pragma once

#include <settings.h>
#include <thread.h>
#include <ui_controller.h>

#include <memory>

class LowBatteryController : public Thread {
 public:
  LowBatteryController();
  ~LowBatteryController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  UiData m_uiData;
  Settings m_settings;
  std::vector<std::unique_ptr<Thread>> m_threads;
};
