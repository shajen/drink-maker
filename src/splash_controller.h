#pragma once

#include <display.h>
#include <thread.h>

class SplashController : public Thread {
 public:
  SplashController(Display& display);
  ~SplashController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  Display& m_display;
};