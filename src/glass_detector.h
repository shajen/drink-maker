#pragma once

#include <Adafruit_VL53L0X.h>
#include <thread.h>

#include <chrono>

class GlassDetector : public Thread {
 public:
  enum class Status { Appeared, Detected, Disappeared, NotDetected };

  GlassDetector();
  ~GlassDetector();

  void loop(const std::chrono::milliseconds& now) override;
  Status status(const std::chrono::milliseconds& now);
  int getDistance() const;
  void setDetectionDistance(int distance);
  void setGlassDetectionDelay(std::chrono::milliseconds delay);
  void setGlassDisappearDelay(std::chrono::milliseconds delay);

 private:
  Adafruit_VL53L0X m_distanceMeter;

  Status m_status;
  std::chrono::milliseconds m_lastDetectedTime;
  std::chrono::milliseconds m_lastNotDetectedTime;
  int m_detectionDistance;
  int m_distance;
  std::chrono::milliseconds m_glassDetectionDelay;
  std::chrono::milliseconds m_glassDisappearDelay;
};