#include <Arduino.h>
#include <logger.h>
#include <status_controller.h>

constexpr auto STATUS_PRINT_INTERVAL = std::chrono::milliseconds(1000);

StatusController::StatusController() : m_frames(0), m_lastPrintStatusTime(0), m_fps(0.0f) {}

StatusController::~StatusController() {}

void StatusController::loop(const std::chrono::milliseconds& now) {
  if (m_lastPrintStatusTime + STATUS_PRINT_INTERVAL <= now) {
    char tmp[100];
    m_fps = 1000.0f * m_frames / (now - m_lastPrintStatusTime).count();
    sprintf(tmp, "fps: %.2f, free: %d B", m_fps, ESP.getFreeHeap());
    log("status", tmp);
    m_frames = 0;
    m_lastPrintStatusTime = now;
  }
  m_frames++;
}

float StatusController::getFps() const { return m_fps; }