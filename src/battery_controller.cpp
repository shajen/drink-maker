#include "battery_controller.h"

#include <Arduino.h>
#include <MAX17043.h>
#include <helpers.h>
#include <logger.h>

#include <algorithm>

constexpr auto LABEL = "battery";
constexpr auto LOW_BATTERY_MAX_PERCENTAGE = 9;

BatteryController::BatteryController() : m_lastReadTime(0), m_lastVoltage(0.0f), m_lastPercentage(0) {
  if (FuelGauge.begin()) {
    log(LABEL, "success, version: %d", FuelGauge.version());
  } else {
    log(LABEL, "error");
  }
  m_lastVoltage = FuelGauge.voltage() / 1000.0f;
  m_lastPercentage = FuelGauge.percent();
}

BatteryController::~BatteryController() = default;

void BatteryController::loop(const std::chrono::milliseconds& now) {
  if (m_lastReadTime + BATTERY_READ_INTERVAL <= now) {
    const auto currentPercentage = FuelGauge.percent();
    if (isLowPercentage(currentPercentage) && !isLowPercentage(m_lastPercentage)) {
      log(LABEL, "low battery detected");
      ESP.restart();
    } else if (!isLowPercentage(currentPercentage) && isLowPercentage(m_lastPercentage)) {
      log(LABEL, "low battery gone");
      ESP.restart();
    }
    m_lastVoltage = FuelGauge.voltage() / 1000.0f;
    m_lastPercentage = currentPercentage;
    m_lastReadTime = now;
  }
}

void BatteryController::reset() {
  log(LABEL, "reset");
  FuelGauge.reset();
  delay(250);
  FuelGauge.quickstart();
  delay(250);
}

float BatteryController::getVoltage() const { return m_lastVoltage; }

int BatteryController::getPercentage() const { return m_lastPercentage; }

bool BatteryController::isLowBattery() const { return isLowPercentage(m_lastPercentage); }

bool BatteryController::isLowPercentage(const int percentage) const { return percentage <= LOW_BATTERY_MAX_PERCENTAGE; }