#include <Arduino.h>
#include <logger.h>
#include <pump_controller.h>

constexpr auto RELAY_PIN = 2;

PumpController::PumpController() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, true);
}

PumpController::~PumpController() {}

void PumpController::loop(const std::chrono::milliseconds& now) { std::ignore = now; }

void PumpController::setEnabled(bool enabled) {
  log("pump", enabled ? "start" : "stop");
  digitalWrite(RELAY_PIN, !enabled);
}
