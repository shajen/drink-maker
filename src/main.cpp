#include <logger.h>
#include <main_controller.h>
#include <settings_controller.h>

#include <memory>

constexpr auto UART_BAUDRATE = 921600;
constexpr auto SDA_PIN = 4;
constexpr auto SCL_PIN = 5;

std::unique_ptr<MainController> mainController;

void setup() {
  Serial.begin(UART_BAUDRATE);
  printf("\n");
  log("main", "init");
  log("main", "drink maker v1.0.0");
  Wire.begin(SDA_PIN, SCL_PIN);

  SettingsController::setup();
  mainController = std::make_unique<MainController>();
}

void loop() {
  delay(1);
  mainController->loop(std::chrono::milliseconds(millis()));
}
