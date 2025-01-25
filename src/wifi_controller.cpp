#include "wifi_controller.h"

#include <logger.h>

constexpr auto LABEL = "WiFi";

constexpr auto SSID = "Vodka";
constexpr auto PASSWORD = "12345678";
constexpr auto NAME = "Vodka";

constexpr auto SAVE_FILE = "/save.json";
constexpr auto SETTINGS_FILE = "/settings.json";
constexpr auto SETTINGS_DATA_FILE = "/settings.data.json";

constexpr auto ROOT_URL = "/";
constexpr auto SETTINGS_URL = "/settings";
constexpr auto SAVE_SETTINGS_URL = "/save_settings";
constexpr auto RESET_SETTINGS_URL = "/reset_settings";
constexpr auto RESTART_URL = "/restart";
constexpr auto ICON_URL = "/favicon.ico";
constexpr auto STYLE_URL = "/style.css";

using namespace std::placeholders;

WifiController::WifiController(Settings& settings, std::function<void()> updateSettingsCallback) : m_settings(settings), m_updateSettingsCallback(updateSettingsCallback), m_portal(m_server) {
  log(LABEL, "init");
  m_config.ticker = false;
  m_config.apid = SSID;
  m_config.password = PASSWORD;
  m_config.homeUri = ROOT_URL;
  m_config.bootUri = AC_ONBOOTURI_HOME;
  m_config.menuItems = 0;
  m_config.hostName = NAME;
  m_config.title = NAME;
  m_config.autoRise = false;
  m_config.immediateStart = true;
  m_config.apip = IPAddress(192, 168, 4, 1);
  m_config.gateway = IPAddress(192, 168, 4, 1);
  m_config.netmask = IPAddress(255, 255, 255, 0);
  m_portal.config(m_config);

  loadPages();
  updateSettings();
  m_server.serveStatic(ICON_URL, LittleFS, ICON_URL);
  m_server.serveStatic(STYLE_URL, LittleFS, STYLE_URL);

  m_server.on(ROOT_URL, std::bind(&WifiController::rootCallback, this));
  m_portal.on(SAVE_SETTINGS_URL, std::bind(&WifiController::saveCallback, this, _1, _2));
  m_server.on(RESET_SETTINGS_URL, std::bind(&WifiController::resetCallback, this));
  m_server.on(RESTART_URL, std::bind(&WifiController::restartCallback, this));
  m_portal.begin();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD);
}

WifiController::~WifiController() = default;

void WifiController::loop(const std::chrono::milliseconds& now) { m_portal.handleClient(); }

void WifiController::loadPages() {
  loadPage(SETTINGS_URL, SETTINGS_FILE, SETTINGS_DATA_FILE);
  loadPage(SAVE_SETTINGS_URL, SAVE_FILE);
}

void WifiController::updateSettings() {
  const auto mode = m_portal.aux(SETTINGS_URL)->getElement<AutoConnectSelect>("mode").value();
  if (mode == "debug") {
    m_settings.m_mode = Mode::Debug;
  } else if (mode == "manual") {
    m_settings.m_mode = Mode::Manual;
  } else {
    m_settings.m_mode = Mode::Auto;
  }
  m_settings.m_distance = m_portal.aux(SETTINGS_URL)->getElement<AutoConnectRange>("distance").value;
  m_settings.m_capacity = m_portal.aux(SETTINGS_URL)->getElement<AutoConnectRange>("capacity").value;
  m_settings.m_brightness = m_portal.aux(SETTINGS_URL)->getElement<AutoConnectRange>("brightness").value;
  m_settings.m_glassDetectionDelay = std::chrono::milliseconds(m_portal.aux(SETTINGS_URL)->getElement<AutoConnectRange>("glass_detection_delay").value);
  m_settings.m_glassDisappearDelay = std::chrono::milliseconds(m_portal.aux(SETTINGS_URL)->getElement<AutoConnectRange>("glass_disappear_delay").value);
  m_updateSettingsCallback();
}

void WifiController::loadPage(const char* uri, const char* templateFile, const char* dataFile) {
  log(LABEL, "loading, uri: %s, template file: %s", uri, templateFile);

  const bool exists = m_portal.aux(uri);
  auto page = exists ? m_portal.aux(uri) : new AutoConnectAux;

  File file = LittleFS.open(templateFile, "r");
  page->load(file);
  file.close();
  if (!exists) {
    m_portal.join(*page);
  }

  if (dataFile && LittleFS.exists(dataFile)) {
    log(LABEL, "loading, uri: %s, data file: %s", uri, dataFile);
    File file = LittleFS.open(dataFile, "r");
    page->loadElement(file);
    file.close();
  }
}

String WifiController::rootCallback() {
  log(LABEL, "root");
  return redirect(SETTINGS_URL);
}

String WifiController::saveCallback(AutoConnectAux& aux, PageArgument& args) {
  log(LABEL, "save settings");
  File file = LittleFS.open(SETTINGS_DATA_FILE, "w");
  aux.referer().saveElement(file, {"mode", "distance", "capacity", "brightness", "glass_detection_delay", "glass_disappear_delay"});
  file.close();
  aux.redirect(SETTINGS_URL);
  updateSettings();
  return "";
}

String WifiController::resetCallback() {
  log(LABEL, "reset");
  LittleFS.remove(SETTINGS_DATA_FILE);
  loadPages();
  updateSettings();
  return redirect(SETTINGS_URL);
}

String WifiController::restartCallback() {
  log(LABEL, "restart");
  m_rebootTimer.once(1.0, []() { ESP.restart(); });
  return redirect(SETTINGS_URL);
}

String WifiController::redirect(const String& url) {
  log(LABEL, "redirect to: %s", url.c_str());
  m_server.sendHeader("Location", url);
  m_server.send(303);
  return "";
}
