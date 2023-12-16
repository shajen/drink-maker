#include <logic/splash_controller.h>

SplashController::SplashController(StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController)
    : LogicController(statusController, display, glassDetector, ledController, pumpController) {
  m_display.setState(Display::State::Splash);
  m_ledController.setState(LedController::State::Splash);
}

SplashController::~SplashController() = default;

void SplashController::loop(const std::chrono::milliseconds&) {}