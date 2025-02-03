#include <Arduino.h>
#include <config.h>
#include <logger.h>
#include <pump_controller.h>

PumpController::PumpController() {
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, true);
}

PumpController::~PumpController() {}

void PumpController::loop(const std::chrono::milliseconds& now) { std::ignore = now; }

void PumpController::setEnabled(bool enabled) {
  log("pump", enabled ? "start" : "stop");
  digitalWrite(PUMP_PIN, !enabled);
}
