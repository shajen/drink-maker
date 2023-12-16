#pragma once

#include <thread.h>

class PumpController : public Thread {
 public:
  PumpController();
  ~PumpController();

  void loop(const std::chrono::milliseconds& now) override;
  void setEnabled(bool enabled);
};
