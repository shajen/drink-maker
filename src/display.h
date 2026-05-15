#pragma once

#include <Adafruit_ST7789.h>
#include <battery_controller.h>
#include <glass_detector.h>
#include <helpers.h>
#include <settings.h>
#include <status_controller.h>
#include <thread.h>

#include <chrono>
#include <string>

class Display : public Thread {
 public:
  enum class State { Splash, Pouring, LowBattery };

  Display(const Settings& settings, const BatteryController& batteryController, const StatusController& statusController, const GlassDetector& glassDetector);
  ~Display();

  void loop(const std::chrono::milliseconds& now) override;
  void setState(const State state);
  void setPouringData(const int counter, const float progress);

 private:
  struct TextBounds {
    int16_t x, y;
    uint16_t width, height;
  };

  void clear();
  void drawImage(const char* path, const int x = 0, const int y = 0, const int width = LCD_WIDTH, const int height = LCD_HEIGHT);
  TextBounds getTextBounds(const int x, const int y, const char* text, const uint8_t size, const uint8_t alignment);
  void drawText(const int x, const int y, const char* oldText, const char* newText, const uint8_t size, const uint16_t color, const uint8_t alignment);
  uint16_t hsvToColor(const float h, const float s, const float v);
  uint16_t stringToColor(const std::string& color);

  Adafruit_ST7789 m_display;
  State m_state;
  const Settings& m_settings;
  const BatteryController& m_batteryController;
  const StatusController& m_statusController;
  const GlassDetector& m_glassDetector;
  uint16_t m_primaryColor;
  uint16_t m_secondaryColor;
  std::chrono::milliseconds m_lastPrintDebugTime;
  LongStaticString m_debugData;
  ShortStaticString m_capacityData;
  ShortStaticString m_batteryData;
  ShortStaticString m_modeData;
  ShortStaticString m_counterData;
  int m_progressHeightData;
};
