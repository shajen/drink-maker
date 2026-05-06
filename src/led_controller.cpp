#include <config.h>
#include <led_controller.h>

LedController::LedController() : m_ws(WS_LEDS_COUNT, WS_LED_PIN, NEO_GRB + NEO_KHZ800), m_state(State::Splash), m_needUpdate(true), m_progress(0.0f) { m_ws.begin(); }

LedController::~LedController() {}

uint32_t wheel(const int position, const int brightnessState) {
  const auto c1 = brightnessState % 512;
  const auto c2 = c1 <= 255 ? c1 : 511 - c1;
  if (position <= 5) {
    return Adafruit_NeoPixel::Color(c2, 0, 0);
  } else {
    return Adafruit_NeoPixel::Color(0, 0, c2);
  }
}

void LedController::loop(const std::chrono::milliseconds& now) {
  if (!m_needUpdate) {
    return;
  }

  if (m_state == State::Off) {
    m_ws.clear();
  } else if (m_state == State::Splash) {
    const auto state = now.count() / SPLASH_UPDATE_INTERVAL.count();
    m_ws.clear();
    for (int i = 0; i < WS_LEDS_COUNT; i++) {
      m_ws.setPixelColor(i, wheel((state + WS_LEDS_COUNT - i) % WS_LEDS_COUNT, now.count() / 3));
    }
  } else if (m_state == State::Full) {
    for (int i = 0; i < WS_LEDS_COUNT; i++) {
      m_ws.setPixelColor(i, 0, 255, 0);
    }
  } else if (m_state == State::Pouring) {
    const auto f0 = m_progress * WS_LEDS_COUNT;
    const auto f1 = static_cast<int>(f0);
    const auto f2 = static_cast<int>((f0 - f1) * 255);
    for (int i = 0; i < f1; ++i) {
      m_ws.setPixelColor(i, 255, 0, 0);
    }
    m_ws.setPixelColor(f1, f2, 0, 0);
    for (int i = f1 + 1; i < WS_LEDS_COUNT; ++i) {
      m_ws.setPixelColor(i, 0, 0, 0);
    }
  }
  m_ws.show();
}

bool LedController::isActive() const { return m_state != State::Off; }

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