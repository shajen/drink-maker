#pragma once

#include <config.h>
#include <thread.h>

#include <array>
#include <chrono>

class BatteryController : public Thread {
 public:
  BatteryController();
  ~BatteryController();

  void loop(const std::chrono::milliseconds& now) override;

  float getVoltage();
  float getPercentage();

 private:
  void readRawVoltage();
  void update();

  std::chrono::milliseconds m_lastReadTime;
  float m_lastVoltage;
  float m_lastPercentage;
  std::array<int, BATTERY_VOLTAGE_AVEREAGE_SAMPLES> m_samples;
  int m_sampleCount;
};
