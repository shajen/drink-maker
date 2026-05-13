#include <config.h>
#include <display.h>
#include <logger.h>

#include <cstring>

constexpr auto MARGIN = 2;
constexpr auto WIDTH_STATUS = 32;

constexpr auto X_CENTER = 2;
constexpr auto Y_CENTER = 4;

Display::Display(const Settings& settings, const BatteryController& batteryController) : m_display(LCD_CS, LCD_DC, LCD_RST), m_settings(settings), m_batteryController(batteryController) {
  SPI.begin(LCD_SCLK, -1, LCD_MOSI, -1);
  SPI.setFrequency(40000000);
  m_display.init(LCD_HEIGHT, LCD_WIDTH);
  m_display.setRotation(1);
  m_display.setTextWrap(false);

  clear();
}

Display::~Display() {}

void Display::loop(const std::chrono::milliseconds& now) {
  if (m_state == State::Pouring) {
    ShortStaticString capacityData;
    ShortStaticString batteryData;
    ShortStaticString modeData;

    sprintf(capacityData.data(), "%d ml", m_settings.m_capacity);
    sprintf(batteryData.data(), "%d %%", m_batteryController.getPercentage());
    sprintf(modeData.data(), m_settings.m_mode == Mode::Auto ? "AUTO" : "MANUAL");

    drawText(MARGIN, MARGIN, m_capacityData.data(), capacityData.data(), 3, m_secondaryColor, 0);
    drawText(-MARGIN - WIDTH_STATUS, MARGIN, m_batteryData.data(), batteryData.data(), 3, m_secondaryColor, 0);
    drawText(MARGIN, -MARGIN, m_modeData.data(), modeData.data(), 3, m_secondaryColor, 0);

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
  m_display.fillScreen(m_backgroundColor);
  if (state == State::Splash) {
    drawText(0, 20, "", "VODKA", 6, m_secondaryColor, X_CENTER);
    drawText(0, 0, "", "CONNECTING", 5, m_primaryColor, X_CENTER | Y_CENTER);
    drawText(0, -20, "", "PEOPLE", 5, m_primaryColor, X_CENTER);
  } else if (state == State::Pouring) {
    clear();
  }
}

void Display::clear() {
  std::memset(m_capacityData.data(), 0, sizeof(m_capacityData));
  std::memset(m_batteryData.data(), 0, sizeof(m_batteryData));
  std::memset(m_modeData.data(), 0, sizeof(m_modeData));
  std::memset(m_counterData.data(), 0, sizeof(m_counterData));
  m_progressHeightData = 0;

  m_backgroundColor = stringToColor(m_settings.m_backgroundColor);
  m_primaryColor = stringToColor(m_settings.m_primaryColor);
  m_secondaryColor = stringToColor(m_settings.m_secondaryColor);

  m_display.fillScreen(m_backgroundColor);
}

void Display::setPouringData(const int counter, const float progress) {
  ShortStaticString counterData;
  sprintf(counterData.data(), "%d", counter);
  drawText(0, 0, m_counterData.data(), counterData.data(), 12, m_primaryColor, X_CENTER | Y_CENTER);
  m_counterData = counterData;

  const auto progressHeight = static_cast<int>(LCD_HEIGHT * progress);
  if (progressHeight != m_progressHeightData) {
    if (progressHeight) {
      const auto color = hsvToColor(progress * 90, 1.0f, 1.0f);
      m_display.fillRect(LCD_WIDTH - WIDTH_STATUS, LCD_HEIGHT - progressHeight, WIDTH_STATUS, progressHeight, color);
    } else {
      m_display.fillRect(LCD_WIDTH - WIDTH_STATUS, 0, WIDTH_STATUS, LCD_HEIGHT, m_backgroundColor);
    }
    m_progressHeightData = progressHeight;
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
    m_display.fillRect(textBounds.x, textBounds.y, textBounds.width, textBounds.height, m_backgroundColor);
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
