#include <display.h>
#include <logger.h>

constexpr auto LCD_ADDRESS = 0x3C;
constexpr auto LCD_WIDTH = 128;
constexpr auto LCD_HEIGHT = 64;

constexpr auto HEIGHT = 16;
constexpr auto WIDTH = 16;

Display::Display() : m_display(LCD_WIDTH, LCD_HEIGHT, &Wire, -1), m_state(Display::State::Splash), m_needUpdate(true), m_progressHeight(0) {
  m_display.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS);
  m_display.setTextColor(SSD1306_WHITE);
  m_display.cp437(true);
  m_display.clearDisplay();
  m_display.display();
}

Display::~Display() {}

void Display::loop(const std::chrono::milliseconds& now) {
  if (!m_needUpdate) {
    return;
  }

  m_needUpdate = false;
  m_display.clearDisplay();
  if (m_state == State::Splash) {
    m_display.setCursor(15, 0);
    m_display.setTextSize(3);
    m_display.printf("VODKA");

    m_display.setCursor(0, 30);
    m_display.setTextSize(2);
    m_display.printf("connecting");

    m_display.setCursor(25, 45);
    m_display.setTextSize(2);
    m_display.printf("people");
  } else if (m_state == State::Pouring) {
    showStatus();
    m_display.fillRect(LCD_WIDTH - WIDTH, LCD_HEIGHT - m_progressHeight, WIDTH, m_progressHeight, SSD1306_WHITE);
  } else if (m_state == State::Debug) {
    m_display.setTextSize(2);

    m_display.setCursor(0, 0);
    m_display.printf("%s", m_editorMode.c_str());

    m_display.setCursor(0, 20);
    if (m_editorLabel.find('\n') == std::string::npos) {
      m_display.printf("%s", m_editorLabel.c_str());
    } else {
      m_display.setTextSize(1);
      m_display.printf("%s", m_editorLabel.c_str());
      m_display.setTextSize(2);
    }

    m_display.setCursor(0, 40);
    m_display.printf("%s", m_editorValue.c_str());
  }
  m_display.display();
}

void Display::setState(const State state) {
  m_state = state;
  m_needUpdate = true;
}

void Display::setPouringData(const std::string& line1, const std::string& line2, float progress) {
  const auto progressHeight = static_cast<int>(LCD_HEIGHT * progress);
  if (m_pouringLine1 != line1 || m_pouringLine2 != line2 || progressHeight != m_progressHeight) {
    m_pouringLine1 = line1;
    m_pouringLine2 = line2;
    m_progressHeight = progressHeight;
    m_needUpdate = true;
  }
}

void Display::setEditorData(const std::string& editorMode, const std::string& editorLabel, const std::string& editorValue) {
  m_editorMode = editorMode;
  m_editorLabel = editorLabel;
  m_editorValue = editorValue;
  m_needUpdate = true;
}

void Display::showStatus() {
  m_display.setCursor(0, 0);
  m_display.setTextSize(6);
  m_display.printf("%s", m_pouringLine1.c_str());

  m_display.setCursor(0, 48);
  m_display.setTextSize(2);
  m_display.printf("%s", m_pouringLine2.c_str());
}