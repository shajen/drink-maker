#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <logic/debug_controller.h>
#include <logic/pouring_controller.h>
#include <logic/splash_controller.h>
#include <main_controller.h>

using namespace std::chrono_literals;

constexpr auto HOSTNAME = "vodka";
constexpr auto OTA_PASSWORD = "12345678";
constexpr auto SPLASH_SCREEN_TIME = 2s;

MainController::MainController()
    : m_isSplash(true),
      m_wifiController(m_settings, std::bind(&MainController::updateSettingsCallback, this)),
      m_logicController(std::make_unique<SplashController>(m_settings, m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController)) {
  m_ledController.setBrightness(m_settings.m_brightness);
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.begin();
  MDNS.begin(HOSTNAME);
}

MainController::~MainController() {}

void MainController::loop(const std::chrono::milliseconds& now) {
  if (m_isSplash && SPLASH_SCREEN_TIME <= now) {
    updateLogicController();
    m_isSplash = false;
  }

  m_wifiController.loop(now);
  m_statusController.loop(now);
  m_display.loop(now);
  m_glassDetector.loop(now);
  m_ledController.loop(now);
  m_pumpController.loop(now);
  m_logicController->loop(now);
  ArduinoOTA.handle();
  MDNS.update();
}

void MainController::updateSettingsCallback() {
  if (!m_isSplash) {
    updateLogicController();
  }
}

void MainController::updateLogicController() {
  m_ledController.setBrightness(m_settings.m_brightness);
  if (m_settings.m_mode == Mode::Debug) {
    m_logicController = std::make_unique<DebugController>(m_settings, m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController);
  } else {
    m_logicController = std::make_unique<PouringController>(m_settings, m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController);
  }
}
