#include <logic/debug_controller.h>

using namespace std::chrono_literals;

constexpr auto MIN_CAPACITY = 5;
constexpr auto MAX_CAPACITY = 200;
constexpr auto STEP_CAPACITY = 5;

constexpr auto MIN_DISTANCE = 1;
constexpr auto MAX_DISTANCE = 50;
constexpr auto STEP_DISTANCE = 1;

constexpr auto MIN_BRIGHTNESS = 25;
constexpr auto MAX_BRIGHTNESS = 250;
constexpr auto STEP_BRIGHTNESS = 25;

constexpr auto MIN_GLASS_DETECTION_DELAY = 100ms;
constexpr auto MAX_GLASS_DETECTION_DELAY = std::chrono::duration_cast<std::chrono::milliseconds>(10s);
constexpr auto STEP_GLASS_DETECTION_DELAY = 100ms;

constexpr auto MIN_GLASS_DISAPPEAR_DELAY = 10ms;
constexpr auto MAX_GLASS_DISAPPEAR_DELAY = std::chrono::duration_cast<std::chrono::milliseconds>(1s);
constexpr auto STEP_GLASS_DISAPPEAR_DELAY = 10ms;

constexpr auto EDIT_LABEL = "EDYCJA";
constexpr auto VIEW_LABEL = "PODGLAD";

namespace {
template <typename T>
void increase(T& value, T step, T max) {
  value = std::min(value + step, max);
}
template <typename T>
void decrease(T& value, T step, T min) {
  value = std::max(value - step, min);
}
}  // namespace

DebugController::DebugController(StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController)
    : LogicController(statusController, display, glassDetector, ledController, pumpController), m_editedType(Type::Mode) {
  m_editedType = static_cast<Type>(SettingsController::load(SettingsController::Type::EditedType, 0));

  m_display.setState(Display::State::Debug);
  m_ledController.setState(LedController::State::Off);
  updateDisplay();
}

DebugController::~DebugController() = default;

void DebugController::loop(const std::chrono::milliseconds& now) {
  if (m_editedType == Type::CurrentDistance) {
    updateDisplay();
  }
}

void DebugController::onButton1Click() {
  if (m_editedType == Type::Capacity) {
    decrease(m_capacity, STEP_CAPACITY, MIN_CAPACITY);
    SettingsController::save(SettingsController::Type::Capacity, m_capacity);
  } else if (m_editedType == Type::Distance) {
    decrease(m_distance, STEP_DISTANCE, MIN_DISTANCE);
    SettingsController::save(SettingsController::Type::Distance, m_distance);
  } else if (m_editedType == Type::Brightness) {
    decrease(m_brightness, STEP_BRIGHTNESS, MIN_BRIGHTNESS);
    SettingsController::save(SettingsController::Type::Brightness, m_brightness);
  } else if (m_editedType == Type::Mode) {
    switchMode();
  } else if (m_editedType == Type::GlassDetectionDelay) {
    decrease(m_glassDetectionDelay, STEP_GLASS_DETECTION_DELAY, MIN_GLASS_DETECTION_DELAY);
    SettingsController::save(SettingsController::Type::GlassDetectionDelay, m_glassDetectionDelay.count());
  } else if (m_editedType == Type::GlassDisappearDelay) {
    decrease(m_glassDisappearDelay, STEP_GLASS_DISAPPEAR_DELAY, MIN_GLASS_DISAPPEAR_DELAY);
    SettingsController::save(SettingsController::Type::GlassDisappearDelay, m_glassDisappearDelay.count());
  }
  updateDisplay();
}

void DebugController::onButton2Click() {
  if (m_editedType == Type::Capacity) {
    increase(m_capacity, STEP_CAPACITY, MAX_CAPACITY);
    SettingsController::save(SettingsController::Type::Capacity, m_capacity);
  } else if (m_editedType == Type::Distance) {
    increase(m_distance, STEP_DISTANCE, MAX_DISTANCE);
    SettingsController::save(SettingsController::Type::Distance, m_distance);
  } else if (m_editedType == Type::Brightness) {
    increase(m_brightness, STEP_BRIGHTNESS, MAX_BRIGHTNESS);
    SettingsController::save(SettingsController::Type::Brightness, m_brightness);
  } else if (m_editedType == Type::Mode) {
    switchMode();
  } else if (m_editedType == Type::GlassDetectionDelay) {
    increase(m_glassDetectionDelay, STEP_GLASS_DETECTION_DELAY, MAX_GLASS_DETECTION_DELAY);
    SettingsController::save(SettingsController::Type::GlassDetectionDelay, m_glassDetectionDelay.count());
  } else if (m_editedType == Type::GlassDisappearDelay) {
    increase(m_glassDisappearDelay, STEP_GLASS_DISAPPEAR_DELAY, MAX_GLASS_DISAPPEAR_DELAY);
    SettingsController::save(SettingsController::Type::GlassDisappearDelay, m_glassDisappearDelay.count());
  }
  updateDisplay();
}

void DebugController::onButton1LongClick() {
  const auto MAX = static_cast<int>(Type::Max);
  m_editedType = static_cast<Type>((static_cast<int>(m_editedType) + MAX - 1) % MAX);
  SettingsController::save(SettingsController::Type::EditedType, static_cast<uint8_t>(m_editedType));
  updateDisplay();
}

void DebugController::onButton2LongClick() {
  const auto MAX = static_cast<int>(Type::Max);
  m_editedType = static_cast<Type>((static_cast<int>(m_editedType) + 1) % MAX);
  SettingsController::save(SettingsController::Type::EditedType, static_cast<uint8_t>(m_editedType));
  updateDisplay();
}

void DebugController::switchMode() {
  if (m_mode == Mode::Auto) {
    log("debug", "set mode manual");
    m_mode = Mode::Manual;
  } else if (m_mode == Mode::Manual) {
    log("debug", "set mode auto");
    m_mode = Mode::Auto;
  }
  SettingsController::save(SettingsController::Type::Mode, static_cast<uint8_t>(m_mode));
}

void DebugController::updateDisplay() {
  char tmpEditLabel[20];
  char tmpViewLabel[20];
  char tmp[20];
  const auto n = static_cast<int>(m_editedType) + 1;
  sprintf(tmpEditLabel, "%s %d", EDIT_LABEL, n);
  sprintf(tmpViewLabel, "%s %d", VIEW_LABEL, n);
  if (m_editedType == Type::Capacity) {
    sprintf(tmp, "%d ml", m_capacity);
    m_display.setEditorData(tmpEditLabel, "POJEMNOSC", tmp);
  } else if (m_editedType == Type::Distance) {
    sprintf(tmp, "%d cm", m_distance);
    m_display.setEditorData(tmpEditLabel, "ODLEGLOSC", tmp);
  } else if (m_editedType == Type::Brightness) {
    sprintf(tmp, "%d", m_brightness);
    m_display.setEditorData(tmpEditLabel, "JASNOSC", tmp);
  } else if (m_editedType == Type::Mode) {
    if (m_mode == Mode::Auto) {
      m_display.setEditorData(tmpEditLabel, "TRYB", "AUTO");
    } else if (m_mode == Mode::Manual) {
      m_display.setEditorData(tmpEditLabel, "TRYB", "RECZNY");
    }
  } else if (m_editedType == Type::CurrentDistance) {
    sprintf(tmp, "%d cm", m_glassDetector.getDistance());
    m_display.setEditorData(tmpViewLabel, "ODLEGLOSC", tmp);
  } else if (m_editedType == Type::GlassDetectionDelay) {
    sprintf(tmp, "%.1f s", m_glassDetectionDelay.count() / 1000.0f);
    m_display.setEditorData(tmpEditLabel, "OPOZ WYKR", tmp);
  } else if (m_editedType == Type::GlassDisappearDelay) {
    sprintf(tmp, "%.2f s", m_glassDisappearDelay.count() / 1000.0f);
    m_display.setEditorData(tmpEditLabel, "OPOZ ZNIKN", tmp);
  }
}