#include <LittleFS.h>
#include <config.h>
#include <display.h>
#include <helpers.h>
#include <logger.h>

#include <cstring>

constexpr auto STATUS_X = 11;
constexpr auto STATUS_Y = 56;
constexpr auto STATUS_WIDTH = 22;
constexpr auto STATUS_HEIGHT = 112;

constexpr auto X_CENTER = 2;
constexpr auto Y_CENTER = 4;

Display::Display(const Settings& settings, const BatteryController& batteryController, const StatusController& statusController, const GlassDetector& glassDetector)
    : m_display(LCD_CS, LCD_DC, LCD_RST),
      m_state(batteryController.isLowBattery() ? State::LowBattery : State::Splash),
      m_settings(settings),
      m_batteryController(batteryController),
      m_statusController(statusController),
      m_glassDetector(glassDetector),
      m_lastPrintDebugTime(0) {
  SPI.begin(LCD_SCLK, -1, LCD_MOSI, -1);
  SPI.setFrequency(40000000);
  m_display.init(LCD_HEIGHT, LCD_WIDTH);
  m_display.enableDisplay(false);
  m_display.setRotation(1);
  m_display.setTextWrap(false);

  clear();
  m_display.enableDisplay(true);
}

Display::~Display() {}

void Display::loop(const std::chrono::milliseconds& now) {
  if (0 < strlen(m_debugData.data()) && !m_settings.m_isDebug) {
    clear();
  }
  if (m_state == State::Pouring) {
    if (m_settings.m_isDebug && m_lastPrintDebugTime + UI_DEBUG_PRINT_INTERVAL <= now) {
      m_lastPrintDebugTime = now;
      size_t size = 0;
      LongStaticString debugData;

      size += sprintf(&debugData[size], "%s %3dc %3de\n", formatDuration(now, false), m_glassDetector.getDistance(), m_glassDetector.getErrorCount());
      size += sprintf(&debugData[size], "%3dk %.2fV %.0ff %c%c\n", ESP.getFreeHeap() / 1024, m_batteryController.getVoltage(), m_statusController.getFps(), getWifiMode(), getWifiStatus());

      drawText(0, 0, m_debugData.data(), debugData.data(), 3, m_primaryColor, 0);

      m_debugData = debugData;
    }

    ShortStaticString capacityData;
    ShortStaticString batteryData;
    ShortStaticString modeData;

    sprintf(capacityData.data(), "%dml", m_settings.m_capacity);
    sprintf(batteryData.data(), "%d%%", m_batteryController.getPercentage(m_settings.m_isDebug));
    sprintf(modeData.data(), m_settings.m_mode == Mode::Auto ? "AUTO" : "MAN ");

    drawText(260, 193, m_capacityData.data(), capacityData.data(), 2, m_secondaryColor, 0);
    drawText(260, 172, m_batteryData.data(), batteryData.data(), 2, m_secondaryColor, 0);
    drawText(260, 217, m_modeData.data(), modeData.data(), 2, m_secondaryColor, 0);

    m_capacityData = capacityData;
    m_batteryData = batteryData;
    m_modeData = modeData;
  } else if (m_state == State::LowBattery) {
    ShortStaticString batteryData;
    if (m_settings.m_isDebug) {
      sprintf(batteryData.data(), "%d%% %.2fV", m_batteryController.getPercentage(m_settings.m_isDebug), m_batteryController.getVoltage());
      drawText(0, 0, m_batteryData.data(), batteryData.data(), 3, ST77XX_WHITE, X_CENTER | Y_CENTER);
    } else {
      sprintf(batteryData.data(), "%d %%", m_batteryController.getPercentage(m_settings.m_isDebug));
      drawText(0, 0, m_batteryData.data(), batteryData.data(), 5, ST77XX_WHITE, X_CENTER | Y_CENTER);
    }
    m_batteryData = batteryData;
  }
}

void Display::setState(const State state) {
  if (m_state == state) {
    return;
  }

  m_state = state;
  clear();
}

void Display::clear() {
  std::memset(m_debugData.data(), 0, sizeof(m_debugData));
  std::memset(m_capacityData.data(), 0, sizeof(m_capacityData));
  std::memset(m_batteryData.data(), 0, sizeof(m_batteryData));
  std::memset(m_modeData.data(), 0, sizeof(m_modeData));
  std::memset(m_counterData.data(), 0, sizeof(m_counterData));
  m_progressHeightData = 0;

  m_primaryColor = stringToColor(m_settings.m_primaryColor);
  m_secondaryColor = stringToColor(m_settings.m_secondaryColor);

  if (m_state == State::Splash) {
    drawImage("/splash.raw");
  } else if (m_state == State::Pouring) {
    drawImage("/background.raw");
  } else if (m_state == State::LowBattery) {
    m_display.fillScreen(ST77XX_BLACK);
    constexpr auto LOW_BATTERY_WIDTH = 260;
    constexpr auto LOW_BATTERY_HEIGHT = 120;
    constexpr auto X = (LCD_WIDTH - LOW_BATTERY_WIDTH) / 2;
    constexpr auto Y = (LCD_HEIGHT - LOW_BATTERY_HEIGHT) / 2;
    drawImage("/low_battery.raw", X, Y, LOW_BATTERY_WIDTH, LOW_BATTERY_HEIGHT);
    drawText(0, 20, "", "LOW BATTERY", 3, ST77XX_RED, X_CENTER);
    drawText(0, 200, "", "PLEASE CHARGE", 3, ST77XX_RED, X_CENTER);
  }
}

void Display::setPouringData(const int counter, const float progress) {
  ShortStaticString counterData;
  sprintf(counterData.data(), "%d", counter);
  drawText(20, 200, m_counterData.data(), counterData.data(), 4, m_primaryColor, 0);
  m_counterData = counterData;

  const auto progressHeight = static_cast<int>(STATUS_HEIGHT * progress);
  if (progressHeight != m_progressHeightData) {
    if (progressHeight) {
      const auto color = hsvToColor(progress * 90, 1.0f, 1.0f);
      m_display.fillRect(STATUS_X, LCD_HEIGHT - STATUS_Y - progressHeight, STATUS_WIDTH, progressHeight, color);
    } else {
      m_display.fillRect(STATUS_X, LCD_HEIGHT - STATUS_Y - STATUS_HEIGHT, STATUS_WIDTH, STATUS_HEIGHT, ST77XX_BLACK);
    }
    m_progressHeightData = progressHeight;
  }
}

void Display::drawImage(const char* path, const int x, const int y, const int width, const int height) {
  File f = LittleFS.open(path, "r");
  uint16_t buffer[width];
  for (int row = y; row < y + height; row++) {
    f.read((uint8_t*)buffer, width * 2);
    m_display.drawRGBBitmap(x, row, buffer, width, 1);
  }
}

Display::TextBounds Display::getTextBounds(const int x, const int y, const char* text, const uint8_t size, const uint8_t alignment) {
  TextBounds textBounds;
  m_display.setTextSize(size);
  m_display.getTextBounds(text, x, y, &textBounds.x, &textBounds.y, &textBounds.width, &textBounds.height);
  if (alignment & X_CENTER) {
    textBounds.x = (LCD_WIDTH - textBounds.width) / 2;
  } else if (0 < x) {
    textBounds.x = x;
  } else if (x < 0) {
    textBounds.x = LCD_WIDTH + x - textBounds.width;
  }

  if (alignment & Y_CENTER) {
    textBounds.y = (LCD_HEIGHT - textBounds.height) / 2;
  } else if (0 < y) {
    textBounds.y = y;
  } else if (y < 0) {
    textBounds.y = LCD_HEIGHT + y - textBounds.height;
  }

  return textBounds;
}

void Display::drawText(const int x, const int y, const char* oldText, const char* newText, const uint8_t size, const uint16_t color, const uint8_t alignment) {
  if (strcmp(oldText, newText) == 0) {
    return;
  }

  const auto textBounds = getTextBounds(x, y, newText, size, alignment);
  m_display.setTextSize(size);
  m_display.setTextColor(color, ST77XX_BLACK);
  m_display.setCursor(textBounds.x, textBounds.y);
  m_display.print(newText);
}

uint16_t Display::hsvToColor(const float h, const float s, const float v) {
  const float c = v * s;
  const float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
  const float m = v - c;

  float r = 0;
  float g = 0;
  float b = 0;

  if (h < 60) {
    r = c;
    g = x;
  } else if (h < 120) {
    g = c;
    r = x;
  } else if (h < 180) {
    g = c;
    b = x;
  } else if (h < 240) {
    b = c;
    g = x;
  } else if (h < 300) {
    b = c;
    r = x;
  } else {
    r = c;
    b = x;
  }

  return m_display.color565(static_cast<int>((r + m) * 255), static_cast<int>((g + m) * 255), static_cast<int>((b + m) * 255));
}

uint16_t Display::stringToColor(const std::string& color) {
  const auto r = std::stoi(color.substr(1, 2), nullptr, 16);
  const auto g = std::stoi(color.substr(3, 2), nullptr, 16);
  const auto b = std::stoi(color.substr(5, 2), nullptr, 16);
  return m_display.color565(r, g, b);
}
