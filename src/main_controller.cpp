#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <config.h>
#include <main_controller.h>
#include <pouring_controller.h>
#include <splash_controller.h>

MainController::MainController()
    : m_isSplash(true),
      m_uiController(
          m_settings, std::bind(&MainController::updateSettingsCallback, this), std::bind(&GlassDetector::getDistance, &m_glassDetector), std::bind(&MainController::startManualPouringCallback, this)),
      m_logicController(std::make_unique<SplashController>(m_display, m_ledController)) {
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
  m_uiController.loop(now);
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

void MainController::startManualPouringCallback() {
  auto pouringController = reinterpret_cast<PouringController*>(m_logicController.get());
  if (pouringController) {
    pouringController->startManualPouring();
  }
}

void MainController::updateLogicController() {
  m_ledController.setBrightness(m_settings.m_brightness);
  m_logicController = std::make_unique<PouringController>(m_settings, m_statusController, m_display, m_glassDetector, m_ledController, m_pumpController);
}
