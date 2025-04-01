#pragma once

#include <thread.h>

#include <chrono>

class BatteryController : public Thread {
 public:
  BatteryController();
  ~BatteryController();

  void loop(const std::chrono::milliseconds& now) override;

  float getVoltage();
  float getPercentage();

 private:
  float getRawVoltage();

  std::chrono::milliseconds m_lastReadTime;
  float m_lastVoltage;
  float m_lastPercentage;
  float m_sum;
  int m_samples;
};
