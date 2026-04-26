#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <ElegantOTA.h>
#include <config.h>
#include <main_controller.h>
#include <pouring_controller.h>
#include <splash_controller.h>

MainController::MainController()
    : m_isSplash(true),
      m_glassDetector(m_uiData.m_distanceErrorCount),
      m_uiController(
          m_batteryController,
          m_statusController,
          m_glassDetector,
          m_uiData,
          m_settings,
          std::bind(&MainController::updateSettingsCallback, this),
          std::bind(&MainController::startManualPouringCallback, this)),
      m_logicController(std::make_unique<SplashController>(m_display, m_ledController)) {
  m_ledController.setBrightness(m_settings.m_brightness);
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.begin();
  MDNS.begin(HOSTNAME);
  ElegantOTA.begin(ESPUI.WebServer());
}

MainController::~MainController() {}

void MainController::loop(const std::chrono::milliseconds& now) {
  if (m_isSplash && SPLASH_SCREEN_TIME <= now) {
    updateLogicController();
    m_isSplash = false;
  }

  if (!m_ledController.isActive() && !m_uiController.isActive() && !m_pumpController.isActive()) {
    m_batteryController.loop(now);
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
  ElegantOTA.loop();
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
  m_logicController = std::make_unique<PouringController>(m_settings, m_batteryController, m_display, m_glassDetector, m_ledController, m_pumpController, m_uiData.m_pourCount);
}
