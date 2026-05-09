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
  int getPercentage() const;

 private:
  float m_lastVoltage;
  int m_lastPercentage;
};
