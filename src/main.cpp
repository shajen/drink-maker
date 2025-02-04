#include <config.h>
#include <logger.h>
#include <main_controller.h>

#include <memory>

std::unique_ptr<MainController> mainController;

void setup() {
  Serial.begin(UART_BAUDRATE);
  Serial.printf("\n");
  log("main", "init");
  log("main", "drink maker v1.0.0");
  Wire.begin(SDA_PIN, SCL_PIN);

  mainController = std::make_unique<MainController>();
}

void loop() {
  delay(1);
  mainController->loop(std::chrono::milliseconds(millis()));
}
