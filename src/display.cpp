#include <LittleFS.h>
#include <config.h>
#include <display.h>
#include <logger.h>

#include <cstring>

constexpr auto STATUS_X = 11;
constexpr auto STATUS_Y = 56;
constexpr auto STATUS_WIDTH = 22;
constexpr auto STATUS_HEIGHT = 112;

constexpr auto X_CENTER = 2;
constexpr auto Y_CENTER = 4;

Display::Display(const Settings& settings, const BatteryController& batteryController, const StatusController& statusController)
    : m_display(LCD_CS, LCD_DC, LCD_RST), m_state(State::Splash), m_settings(settings), m_batteryController(batteryController), m_statusController(statusController) {
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
  if (m_state == State::Pouring) {
    if (m_settings.m_isDebug) {
      ShortStaticString debugData;
      sprintf(debugData.data(), "%s %.2fV %.0f", formatDuration(now, false), m_batteryController.getVoltage(), m_statusController.getFps());
      drawText(0, 0, m_debugData.data(), debugData.data(), 3, m_primaryColor, 0);
      m_debugData = debugData;
    }

    ShortStaticString capacityData;
    ShortStaticString batteryData;
    ShortStaticString modeData;

    const auto batteryPercentage = m_settings.m_isDebug ? m_batteryController.getPercentage() : std::min(m_batteryController.getPercentage(), 100);
    sprintf(capacityData.data(), "%dml", m_settings.m_capacity);
    sprintf(batteryData.data(), "%d%%", batteryPercentage);
    sprintf(modeData.data(), m_settings.m_mode == Mode::Auto ? "AUTO" : "MANUAL");

    drawText(260, 193, m_capacityData.data(), capacityData.data(), 2, m_secondaryColor, 0);
    drawText(260, 172, m_batteryData.data(), batteryData.data(), 2, m_secondaryColor, 0);
    drawText(260, 217, m_modeData.data(), modeData.data(), 2, m_secondaryColor, 0);

    m_capacityData = capacityData;
    m_batteryData = batteryData;
    m_modeData = modeData;
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

void Display::drawImage(const char* path) {
  File f = LittleFS.open(path, "r");
  uint16_t buffer[LCD_WIDTH];
  for (int y = 0; y < LCD_HEIGHT; y++) {
    f.read((uint8_t*)buffer, LCD_WIDTH * 2);
    m_display.drawRGBBitmap(0, y, buffer, LCD_WIDTH, 1);
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

  m_display.setTextSize(size);
  m_display.setTextColor(color);

  if (0 < strlen(oldText)) {
    const auto textBounds = getTextBounds(x, y, oldText, size, alignment);
    m_display.fillRect(textBounds.x, textBounds.y, textBounds.width, textBounds.height, ST77XX_BLACK);
  }

  const auto textBounds = getTextBounds(x, y, newText, size, alignment);
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
