#pragma once

#include <AutoConnect.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <thread.h>

#include <chrono>

class WifiController : public Thread {
 public:
  WifiController();
  ~WifiController();

  void loop(const std::chrono::milliseconds& now) override;

 private:
  void loadPages();
  void loadPage(const char* uri, const char* templateFile, const char* dataFile = nullptr);

  String redirect(const String& url);
  String rootCallback();
  String saveCallback(AutoConnectAux& aux, PageArgument& args);
  String resetCallback();
  String restartCallback();

  Ticker m_rebootTimer;
  ESP8266WebServer m_server;
  AutoConnectConfig m_config;
  AutoConnect m_portal;
};
