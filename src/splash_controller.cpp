#include <splash_controller.h>

SplashController::SplashController(Display& display, LedController& ledController) : m_display(display), m_ledController(ledController) {
  m_display.setState(Display::State::Splash);
  m_ledController.setState(LedController::State::Splash);
}

SplashController::~SplashController() = default;

void SplashController::loop(const std::chrono::milliseconds&) {}