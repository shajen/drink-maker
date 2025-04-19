#include "wifi_controller.h"

#include <WiFi.h>
#include <config.h>
#include <logger.h>

constexpr auto LABEL = "WiFi";

WifiController::WifiController() {
  log(LABEL, "init");

  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD);
}

WifiController::~WifiController() = default;

void WifiController::loop(const std::chrono::milliseconds& now) { std::ignore = now; }
