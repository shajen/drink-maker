#include <Wire.h>
#include <config.h>
#include <logger.h>
#include <low_battery_controller.h>
#include <main_controller.h>

#include <memory>

std::unique_ptr<Thread> thread;

void setup() {
  Serial.begin(UART_BAUDRATE);
  Serial.printf("\n");
  log("main", "init");
  log("main", "hostname: %s, build time: %s, version: %s, commit: %s", HOSTNAME, BUILD_TIME, GIT_TAG, GIT_COMMIT);
  Wire.begin(SDA_PIN, SCL_PIN);
  LittleFS.begin();

  BatteryController batteryController;
  if (batteryController.isLowBattery()) {
    thread = std::make_unique<LowBatteryController>();
  } else {
    thread = std::make_unique<MainController>();
  }
}

void loop() {
  delay(1);
  thread->loop(std::chrono::milliseconds(millis()));
}
