#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <ElegantOTA.h>
#include <config.h>
#include <main_controller.h>

MainController::MainController()
    : m_isSplash(true),
      m_glassDetector(m_uiData.m_distanceErrorCount),
      m_display(m_settings, m_batteryController, m_statusController, m_glassDetector),
      m_uiController(
          m_batteryController,
          m_statusController,
          m_glassDetector,
          m_uiData,
          m_settings,
          std::bind(&MainController::updateSettingsCallback, this),
          std::bind(&MainController::startManualPouringCallback, this)) {
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.begin();
  MDNS.begin(HOSTNAME);
  ElegantOTA.begin(ESPUI.WebServer());
}

MainController::~MainController() {}

void MainController::loop(const std::chrono::milliseconds& now) {
  std::unique_lock<std::mutex> lock(m_mutex);
  if (m_isSplash && SPLASH_SCREEN_TIME <= now) {
    updatePouringController();
    m_isSplash = false;
  }

  m_batteryController.loop(now);
  m_wifiController.loop(now);
  m_uiController.loop(now);
  m_statusController.loop(now);
  m_display.loop(now);
  m_glassDetector.loop(now);
  m_pumpController.loop(now);
  if (m_pouringController) {
    m_pouringController->loop(now);
  }
  ArduinoOTA.handle();
  ElegantOTA.loop();
}

void MainController::updateSettingsCallback() {
  std::unique_lock<std::mutex> lock(m_mutex);
  m_pumpController.setEnabled(false);
  if (!m_isSplash) {
    updatePouringController();
  }
}

void MainController::startManualPouringCallback() {
  std::unique_lock<std::mutex> lock(m_mutex);
  if (m_pouringController) {
    m_pouringController->startManualPouring();
  }
}

void MainController::updatePouringController() {
  m_pouringController = std::make_unique<PouringController>(m_settings, m_batteryController, m_display, m_glassDetector, m_pumpController, m_uiData.m_pourCount);
}
