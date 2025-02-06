#include "settings.h"

#include <ArduinoJson.h>
#include <LittleFS.h>
#include <config.h>
#include <logger.h>

constexpr auto LABEL = "settings";

String toString(const JsonDocument& json) {
  String tmp;
  serializeJson(json, tmp);
  return tmp;
}

Settings::Settings() {
  load(DEFAULT_SETTINGS_FILE);
  load(USER_SETTINGS_FILE);
}

void Settings::save() { save(USER_SETTINGS_FILE); }

void Settings::reset() {
  LittleFS.remove(USER_SETTINGS_FILE);
  load(DEFAULT_SETTINGS_FILE);
}

void Settings::load(const char* filename) {
  File file = LittleFS.open(filename, "r");
  if (file) {
    log(LABEL, "reading from file: %s", filename);
    JsonDocument json;
    deserializeJson(json, file);
    m_mode = static_cast<Mode>(json["mode"].as<int>());
    m_distance = json["distance"].as<int>();
    m_capacity = json["capacity"].as<int>();
    m_brightness = json["brightness"].as<int>();
    m_glassDetectionDelay = std::chrono::milliseconds(json["glass_detection_delay"].as<int>());
    m_glassDisappearDelay = std::chrono::milliseconds(json["glass_disappear_delay"].as<int>());
    log(LABEL, "%s", toString(json).c_str());
    file.close();
  } else {
    log(LABEL, "can not open file: %s", filename);
  }
}

void Settings::save(const char* filename) {
  File file = LittleFS.open(filename, "w");
  if (file) {
    log(LABEL, "writing to file: %s", filename);
    JsonDocument json;
    json["mode"] = static_cast<int>(m_mode);
    json["distance"] = m_distance;
    json["capacity"] = m_capacity;
    json["brightness"] = m_brightness;
    json["glass_detection_delay"] = m_glassDetectionDelay.count();
    json["glass_disappear_delay"] = m_glassDisappearDelay.count();
    serializeJson(json, file);
    log(LABEL, "%s", toString(json).c_str());
    file.close();
  } else {
    log(LABEL, "can not open file: %s", filename);
  }
}
