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

  void reset();

  float getVoltage() const;
  int getPercentage(const bool& debug) const;
  bool isLowBattery() const;

 private:
  bool isLowPercentage(const float percentage) const;

  std::chrono::milliseconds m_lastReadTime;
  float m_lastVoltage;
  float m_lastPercentage;
};
