#pragma once

#include <thread.h>

#include <chrono>

class StatusController : public Thread {
 public:
  StatusController();
  ~StatusController();

  void loop(const std::chrono::milliseconds& now) override;
  float getFps() const;

 private:
  uint64_t m_frames = 0;
  std::chrono::milliseconds m_lastPrintStatusTime;
  float m_fps;
};