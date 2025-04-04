#pragma once

#include <thread.h>

class PumpController : public Thread {
 public:
  PumpController();
  ~PumpController();

  void loop(const std::chrono::milliseconds& now) override;
  bool isActive() const;
  void setEnabled(bool enabled);

 private:
  bool m_isActive;
};
