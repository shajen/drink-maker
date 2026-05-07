#include <splash_controller.h>

SplashController::SplashController(Display& display) : m_display(display) { m_display.setState(Display::State::Splash); }

SplashController::~SplashController() = default;

void SplashController::loop(const std::chrono::milliseconds&) {}