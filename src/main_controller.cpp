#include <logic/debug_controller.h>
#include <logic/pouring_controller.h>
#include <logic/splash_controller.h>
#include <main_controller.h>

using namespace std::chrono_literals;

constexpr auto SPLASH_SCREEN_TIME = 2s;

MainController::MainController() : m_logicController(std::make_unique<SplashController>(m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController)), m_isSplash(true) {}

MainController::~MainController() {}

void MainController::loop(const std::chrono::milliseconds& now) {
  if (m_isSplash && SPLASH_SCREEN_TIME <= now) {
    m_logicController = std::make_unique<PouringController>(m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController);
    m_isSplash = false;
  }

  m_statusController.loop(now);
  m_display.loop(now);
  m_glassDetector.loop(now);
  m_ledController.loop(now);
  m_pumpController.loop(now);
  m_logicController->loop(now);
}

bool MainController::switchDebug() {
  if (m_isSplash && std::chrono::milliseconds(millis()) < SPLASH_SCREEN_TIME) {
    m_logicController = std::make_unique<DebugController>(m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController);
    m_isSplash = false;
    return true;
  } else {
    return false;
  }
}
