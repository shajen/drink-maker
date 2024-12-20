#include <logic/debug_controller.h>
#include <logic/pouring_controller.h>
#include <logic/splash_controller.h>
#include <main_controller.h>

using namespace std::chrono_literals;

constexpr auto SPLASH_SCREEN_TIME = 2s;
constexpr auto BUTTON1_PIN = 12;
constexpr auto BUTTON2_PIN = 0;

MainController::MainController()
    : m_button1(BUTTON1_PIN, true),
      m_button2(BUTTON2_PIN, true),
      m_logicController(std::make_unique<SplashController>(m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController)),
      m_isSplash(true) {
  m_button1.setClickMs(10);
  m_button1.setIdleMs(10);
  m_button1.setDebounceMs(10);
  m_button1.setPressMs(500);

  m_button2.setClickMs(10);
  m_button2.setIdleMs(10);
  m_button2.setDebounceMs(10);
  m_button2.setPressMs(500);

  auto button1Click = [](void* p) {
    auto c = reinterpret_cast<MainController*>(p);
    if (!c->switchDebug()) {
      c->m_logicController->onButton1Click();
    }
  };
  auto button2Click = [](void* p) {
    auto c = reinterpret_cast<MainController*>(p);
    if (!c->switchDebug()) {
      c->m_logicController->onButton2Click();
    }
  };
  auto button1LongClick = [](void* p) {
    auto c = reinterpret_cast<MainController*>(p);
    if (!c->factoryReset()) {
      c->m_logicController->onButton1LongClick();
    }
  };
  auto button2LongClick = [](void* p) {
    auto c = reinterpret_cast<MainController*>(p);
    if (!c->factoryReset()) {
      c->m_logicController->onButton2LongClick();
    }
  };
  auto button1LongStop = [](void* p) {
    auto c = reinterpret_cast<MainController*>(p);
    if (!c->factoryReset()) {
      c->m_logicController->onButton1LongStop();
    }
  };
  auto button2LongStop = [](void* p) {
    auto c = reinterpret_cast<MainController*>(p);
    if (!c->factoryReset()) {
      c->m_logicController->onButton2LongStop();
    }
  };

  m_button1.attachClick(button1Click, this);
  m_button2.attachClick(button2Click, this);
  m_button1.attachLongPressStart(button1LongClick, this);
  m_button2.attachLongPressStart(button2LongClick, this);
  m_button1.attachLongPressStop(button1LongStop, this);
  m_button2.attachLongPressStop(button2LongStop, this);
}

MainController::~MainController() {}

void MainController::loop(const std::chrono::milliseconds& now) {
  if (m_isSplash && SPLASH_SCREEN_TIME <= now) {
    m_logicController = std::make_unique<PouringController>(m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController);
    m_isSplash = false;
  }

  m_button1.tick();
  m_button2.tick();

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

bool MainController::factoryReset() {
  if (m_isSplash && m_glassDetector.getDistance() <= 3 && std::chrono::milliseconds(millis()) < SPLASH_SCREEN_TIME) {
    SettingsController::factoryReset();
    m_display.setState(Display::State::Debug);
    m_display.setEditorData("USTAWIENIA", "FABRYCZNE", "");
    m_ledController.setState(LedController::State::Off);
    m_isSplash = false;
    return true;
  } else {
    return false;
  }
}
