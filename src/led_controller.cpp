#include <led_controller.h>

constexpr auto WS_LED_PIN = 14;
constexpr auto LEDS_COUNT = 12;
constexpr auto SPLASH_UPDATE_INTERVAL = std::chrono::milliseconds(50);

LedController::LedController() : m_ws(LEDS_COUNT, WS_LED_PIN, NEO_GRB + NEO_KHZ800), m_state(State::Splash), m_needUpdate(true), m_progress(0.0f), m_splashLastLed(0), m_splashLastUpdate(0) {
  m_ws.begin();
}

LedController::~LedController() {}

void LedController::loop(const std::chrono::milliseconds& now) {
  if (!m_needUpdate) {
    return;
  }

  if (m_state == State::Off) {
    m_ws.clear();
  } else if (m_state == State::Splash) {
    if (m_splashLastUpdate + SPLASH_UPDATE_INTERVAL <= now) {
      m_ws.clear();
      m_ws.setPixelColor(m_splashLastLed, 0, 0, 255);
      m_splashLastLed = (m_splashLastLed + 1) % LEDS_COUNT;
      m_splashLastUpdate = now;
    }
  } else if (m_state == State::Full) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      m_ws.setPixelColor(i, 0, 255, 0);
    }
  } else if (m_state == State::Pouring) {
    const auto f0 = m_progress * LEDS_COUNT;
    const auto f1 = static_cast<int>(f0);
    const auto f2 = static_cast<int>((f0 - f1) * 255);
    for (int i = 0; i < f1; ++i) {
      m_ws.setPixelColor(i, 255, 0, 0);
    }
    m_ws.setPixelColor(f1, f2, 0, 0);
    for (int i = f1 + 1; i < LEDS_COUNT; ++i) {
      m_ws.setPixelColor(i, 0, 0, 0);
    }
  }
  m_ws.show();
}

void LedController::setState(const State state) {
  if (m_state != state) {
    m_state = state;
    m_needUpdate = true;
  }
}

void LedController::setPouringData(const float progress) {
  m_progress = progress;
  m_needUpdate = true;
}

void LedController::setBrightness(int brightness) { m_ws.setBrightness(brightness); }