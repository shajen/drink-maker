#include "battery_controller.h"

#include <Arduino.h>
#include <config.h>
#include <helpers.h>
#include <logger.h>

constexpr auto LABEL = "battery";

BatteryController::BatteryController() : m_lastReadTime(0ms), m_lastVoltage(0.0), m_lastPercentage(0.0), m_sum(0), m_samples(0) {}

BatteryController::~BatteryController() = default;

void BatteryController::loop(const std::chrono::milliseconds& now) {
  if (m_lastReadTime + BATTERY_VOLTAGE_READ_INTERVAL <= now) {
    m_lastReadTime = now;
    m_sum += getRawVoltage();
    m_samples++;
  }

  if (m_samples == BATTERY_VOLTAGE_AVEREAGE_SAMPLES) {
    m_lastVoltage = m_sum / m_samples;
    m_lastPercentage = voltageToPercentage(m_lastVoltage);
    m_sum = 0;
    m_samples = 0;
    log(LABEL, "voltage: %.2f, percentage: %.2f", m_lastVoltage, m_lastPercentage);
  }
}

float BatteryController::getVoltage() { return m_lastVoltage; }

float BatteryController::getPercentage() { return m_lastPercentage; }

float BatteryController::getRawVoltage() {
  constexpr auto FACTOR = 1.0 / 1023.0 * 3.2 * BATTERY_VOLTAGE_DIVIDER_FACTOR;
  const auto rawVoltage = analogRead(BATTERY_VOLTAGE_PIN) * FACTOR;
  return rawVoltage;
}
