#include <config.h>
#include <logger.h>
#include <main_controller.h>

#include <memory>

std::unique_ptr<MainController> mainController;

void setup() {
  Serial.begin(UART_BAUDRATE);
  Serial.printf("\n");
  log("main", "init");
  log("main", "hostname: %s, build time: %s, version: %s, commit: %s", HOSTNAME, BUILD_TIME, GIT_TAG, GIT_COMMIT);
  Wire.begin(SDA_PIN, SCL_PIN);
  LittleFS.begin();

  mainController = std::make_unique<MainController>();
}

void loop() {
  delay(1);
  mainController->loop(std::chrono::milliseconds(millis()));
}
