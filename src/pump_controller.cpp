#include <Arduino.h>
#include <config.h>
#include <logger.h>
#include <pump_controller.h>

PumpController::PumpController() : m_isActive(false) {
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, true);
}

PumpController::~PumpController() {}

void PumpController::loop(const std::chrono::milliseconds& now) { std::ignore = now; }

bool PumpController::isActive() const { return m_isActive; }

void PumpController::setEnabled(bool enabled) {
  log("pump", enabled ? "start" : "stop");
  m_isActive = enabled;
  digitalWrite(PUMP_PIN, !enabled);
}
