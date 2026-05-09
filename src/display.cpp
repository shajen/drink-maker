#include <config.h>
#include <display.h>
#include <logger.h>

#include <cstring>

constexpr auto MARGIN = 2;
constexpr auto WIDTH_STATUS = 32;
constexpr auto BACKGROUND = ST77XX_WHITE;

constexpr auto NONE = 0;
constexpr auto X_CENTER = 2;
constexpr auto Y_CENTER = 4;

struct TextBounds {
  int16_t x, y;
  uint16_t width, height;
};

TextBounds getTextBounds(Adafruit_ST7789& display, const int x, const int y, const char* text, const uint8_t size, const uint8_t alignment) {
  TextBounds textBounds;
  display.setTextSize(size);
  display.getTextBounds(text, x, y, &textBounds.x, &textBounds.y, &textBounds.width, &textBounds.height);
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

void drawText(Adafruit_ST7789& display, const int x, const int y, const char* oldText, const char* newText, const uint8_t size, const uint16_t color, const uint8_t alignment) {
  if (strcmp(oldText, newText) == 0) {
    return;
  }

  display.setTextSize(size);
  display.setTextColor(color);

  if (0 < strlen(oldText)) {
    const auto textBounds = getTextBounds(display, x, y, oldText, size, alignment);
    display.fillRect(textBounds.x, textBounds.y, textBounds.width, textBounds.height, BACKGROUND);
  }

  const auto textBounds = getTextBounds(display, x, y, newText, size, alignment);
  display.setCursor(textBounds.x, textBounds.y);
  display.print(newText);
}

uint16_t hsvToColor(Adafruit_ST7789& display, const float h, const float s, const float v) {
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

  return display.color565(static_cast<int>((r + m) * 255), static_cast<int>((g + m) * 255), static_cast<int>((b + m) * 255));
}

Display::Display(const Settings& settings, const BatteryController& batteryController) : m_display(LCD_CS, LCD_DC, LCD_RST), m_settings(settings), m_batteryController(batteryController) {
  SPI.begin(LCD_SCLK, -1, LCD_MOSI, -1);
  SPI.setFrequency(40000000);
  m_display.init(LCD_HEIGHT, LCD_WIDTH);
  m_display.setRotation(1);
  m_display.fillScreen(BACKGROUND);
  m_display.setTextWrap(false);
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

    drawText(m_display, MARGIN, MARGIN, m_capacityData.data(), capacityData.data(), 3, ST77XX_BLUE, 0);
    drawText(m_display, -MARGIN - WIDTH_STATUS, MARGIN, m_batteryData.data(), batteryData.data(), 3, ST77XX_BLUE, 0);
    drawText(m_display, MARGIN, -MARGIN, m_modeData.data(), modeData.data(), 3, ST77XX_BLUE, 0);

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
  m_display.fillScreen(BACKGROUND);
  if (state == State::Splash) {
    drawText(m_display, 0, 20, "", "VODKA", 6, ST77XX_BLUE, X_CENTER);
    drawText(m_display, 0, 0, "", "CONNECTING", 5, ST77XX_RED, X_CENTER | Y_CENTER);
    drawText(m_display, 0, -20, "", "PEOPLE", 5, ST77XX_RED, X_CENTER);
  } else if (state == State::Pouring) {
    std::memset(m_capacityData.data(), 0, sizeof(m_capacityData));
    std::memset(m_batteryData.data(), 0, sizeof(m_batteryData));
    std::memset(m_modeData.data(), 0, sizeof(m_modeData));
    std::memset(m_counterData.data(), 0, sizeof(m_counterData));
  }
}

void Display::setPouringData(const int counter, const float progress) {
  ShortStaticString counterData;
  sprintf(counterData.data(), "%d", counter);
  drawText(m_display, 0, 0, m_counterData.data(), counterData.data(), 12, ST77XX_RED, X_CENTER | Y_CENTER);
  m_counterData = counterData;

  const auto progressHeight = static_cast<int>(LCD_HEIGHT * progress);
  if (progressHeight) {
    const auto color = hsvToColor(m_display, progress * 90, 1.0f, 1.0f);
    m_display.fillRect(LCD_WIDTH - WIDTH_STATUS, LCD_HEIGHT - progressHeight, WIDTH_STATUS, progressHeight, color);
  } else {
    m_display.fillRect(LCD_WIDTH - WIDTH_STATUS, 0, WIDTH_STATUS, LCD_HEIGHT, BACKGROUND);
  }
}
