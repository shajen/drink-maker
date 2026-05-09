#include "battery_controller.h"

#include <Arduino.h>
#include <MAX17043.h>
#include <helpers.h>
#include <logger.h>

#include <algorithm>

constexpr auto LABEL = "battery";

BatteryController::BatteryController() : m_lastVoltage(0.0f), m_lastPercentage(0) {
  if (FuelGauge.begin()) {
    log(LABEL, "success, version: %d", FuelGauge.version());
  } else {
    log(LABEL, "error");
  }
}

BatteryController::~BatteryController() = default;

void BatteryController::loop(const std::chrono::milliseconds& now) {
  m_lastVoltage = FuelGauge.voltage() / 1000.0f;
  m_lastPercentage = FuelGauge.percent();
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
