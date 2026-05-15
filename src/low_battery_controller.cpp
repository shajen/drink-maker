#include "low_battery_controller.h"

#include <battery_controller.h>
#include <display.h>
#include <glass_detector.h>
#include <status_controller.h>

LowBatteryController::LowBatteryController() {
  auto statusController = std::make_unique<StatusController>();
  auto batteryController = std::make_unique<BatteryController>();
  auto glassDetector = std::make_unique<GlassDetector>(m_uiData.m_distanceErrorCount);
  auto display = std::make_unique<Display>(m_settings, *batteryController, *statusController, *glassDetector);

  m_threads.push_back(std::move(statusController));
  m_threads.push_back(std::move(batteryController));
  m_threads.push_back(std::move(glassDetector));
  m_threads.push_back(std::move(display));
}

LowBatteryController::~LowBatteryController() = default;

void LowBatteryController::loop(const std::chrono::milliseconds& now) {
  for (auto& thread : m_threads) {
    thread->loop(now);
  }
}
