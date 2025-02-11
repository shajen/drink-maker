#pragma once

#include <display.h>
#include <led_controller.h>
#include <thread.h>

class SplashController : public Thread {
 public:
  SplashController(Display& display, LedController& ledController);
  ~SplashController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  Display& m_display;
  LedController& m_ledController;
};