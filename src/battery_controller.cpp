#include "battery_controller.h"

#include <Arduino.h>
#include <helpers.h>
#include <logger.h>

#include <algorithm>

constexpr auto LABEL = "battery";

float sampleToVoltage(const int& milliVolts) { return static_cast<float>(milliVolts) / 1000.0f * BATTERY_VOLTAGE_DIVIDER_FACTOR; }

BatteryController::BatteryController()
    : m_lastReadTime(0ms), m_lastVoltage(sampleToVoltage(analogReadMilliVolts(BATTERY_VOLTAGE_PIN))), m_lastPercentage(voltageToPercentage(m_lastVoltage)), m_sampleCount(0) {}

BatteryController::~BatteryController() = default;

void BatteryController::loop(const std::chrono::milliseconds& now) {
  if (m_lastReadTime + BATTERY_VOLTAGE_READ_INTERVAL <= now) {
    m_lastReadTime = now;
    readRawVoltage();

    if (m_sampleCount == BATTERY_VOLTAGE_AVEREAGE_SAMPLES) {
      update();
      log(LABEL, "voltage: %.2f, percentage: %.2f", m_lastVoltage, m_lastPercentage);
    }
  }
}

float BatteryController::getVoltage() const { return m_lastVoltage; }

int BatteryController::getPercentage() const { return m_lastPercentage; }

void BatteryController::readRawVoltage() { m_samples[m_sampleCount++] = analogReadMilliVolts(BATTERY_VOLTAGE_PIN); }

void BatteryController::update() {
  constexpr auto MIDDLE = BATTERY_VOLTAGE_AVEREAGE_SAMPLES / 2;

  std::nth_element(m_samples.begin(), m_samples.begin() + MIDDLE, m_samples.end());
  const auto rawVoltage = sampleToVoltage(m_samples[MIDDLE]);

  m_lastVoltage = rawVoltage;
  m_lastPercentage = voltageToPercentage(m_lastVoltage);
  m_sampleCount = 0;
}