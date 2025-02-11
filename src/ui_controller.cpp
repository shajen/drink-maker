#include "ui_controller.h"

#include <config.h>
#include <converter.h>
#include <logger.h>

using namespace std::placeholders;

constexpr auto LABEL = "ui";

const auto EMPTY_CALLBACK = [](Control*, int) {};
constexpr auto VIEW_CONTROL_COLOR = ControlColor::Peterriver;
constexpr auto NORMAL_BUTTON_CONTROL_COLOR = ControlColor::Emerald;
constexpr auto DANGER_BUTTON_CONTROL_COLOR = ControlColor::Alizarin;

template <typename T>
uint16_t createSelect(const uint16_t parent, const char* name, T& value, std::function<void()> changeValueCallback, std::vector<std::function<void()>>& readSettingsCallbacks) {
  const auto control = ESPUI.addControl(ControlType::Select, name, toString<T>(value), VIEW_CONTROL_COLOR, parent, [&value, changeValueCallback](Control* control, int) {
    value = fromString<T>(control->value);
    changeValueCallback();
  });
  readSettingsCallbacks.push_back([&value, control]() { ESPUI.updateControlValue(control, toString<T>(value)); });
  return control;
}

template <typename T>
uint16_t createSlider(
    const uint16_t parent,
    const char* name,
    T& value,
    const int min,
    const int max,
    const int step,
    std::function<void()> changeValueCallback,
    std::vector<std::function<void()>>& readSettingsCallbacks) {
  const auto control = ESPUI.addControl(ControlType::Slider, name, toString<T>(value), VIEW_CONTROL_COLOR, parent, [&value, changeValueCallback](Control* control, int) {
    value = fromString<T>(control->value);
    changeValueCallback();
  });
  readSettingsCallbacks.push_back([&value, control]() { ESPUI.updateControlValue(control, toString<T>(value)); });
  ESPUI.addControl(ControlType::Min, "min", String(min), VIEW_CONTROL_COLOR, control, EMPTY_CALLBACK);
  ESPUI.addControl(ControlType::Max, "max", String(max), VIEW_CONTROL_COLOR, control, EMPTY_CALLBACK);
  ESPUI.addControl(ControlType::Step, "step", String(step), VIEW_CONTROL_COLOR, control, EMPTY_CALLBACK);
  return control;
}

UiController::UiController(Settings& settings, std::function<void()> updateSettingsCallback, std::function<int()> getDistanceCallback, std::function<void()> manualPourCallback)
    : m_settings(settings),
      m_updateSettingsCallback(updateSettingsCallback),
      m_getDistanceCallback(getDistanceCallback),
      m_manualPourCallback(manualPourCallback),
      m_isDebugTab(false),
      m_lastPrintStatusTime(0) {
  const auto basicTab = ESPUI.addControl(ControlType::Tab, "basic settings", "basic settings", VIEW_CONTROL_COLOR, Control::noParent, [this](Control*, int) { m_isDebugTab = false; });
  const auto advancedTab = ESPUI.addControl(ControlType::Tab, "advanced settings", "advanced settings", VIEW_CONTROL_COLOR, Control::noParent, [this](Control*, int) { m_isDebugTab = false; });
  const auto debugTab = ESPUI.addControl(ControlType::Tab, "debug", "debug", VIEW_CONTROL_COLOR, Control::noParent, [this](Control*, int) { m_isDebugTab = true; });

  const auto modeSelect = createSelect(basicTab, "mode", m_settings.m_mode, std::bind(&UiController::saveSettings, this), m_readSettingsCallbacks);
  ESPUI.addControl(ControlType::Option, "auto", toString<Mode>(Mode::Auto), VIEW_CONTROL_COLOR, modeSelect);
  ESPUI.addControl(ControlType::Option, "manual", toString<Mode>(Mode::Manual), VIEW_CONTROL_COLOR, modeSelect);

  createSlider(basicTab, "capacity [ml]", m_settings.m_capacity, 10, 100, 5, std::bind(&UiController::saveSettings, this), m_readSettingsCallbacks);
  m_manualPourButton = ESPUI.addControl(ControlType::Button, "manual pour", "run", NORMAL_BUTTON_CONTROL_COLOR, basicTab, std::bind(&UiController::manualPour, this, _1, _2));
  ESPUI.addControl(ControlType::Button, "default settings", "run", DANGER_BUTTON_CONTROL_COLOR, basicTab, std::bind(&UiController::resetSettings, this, _1, _2));

  createSlider(advancedTab, "detection distance [cm]", m_settings.m_distance, 1, 100, 1, std::bind(&UiController::saveSettings, this), m_readSettingsCallbacks);
  createSlider(advancedTab, "brightness", m_settings.m_brightness, 25, 250, 25, std::bind(&UiController::saveSettings, this), m_readSettingsCallbacks);
  createSlider(advancedTab, "glass detection delay [ms]", m_settings.m_glassDetectionDelay, 100, 10000, 100, std::bind(&UiController::saveSettings, this), m_readSettingsCallbacks);
  createSlider(advancedTab, "glass disappear delay [ms]", m_settings.m_glassDisappearDelay, 10, 1000, 10, std::bind(&UiController::saveSettings, this), m_readSettingsCallbacks);
  ESPUI.addControl(ControlType::Button, "default settings", "run", DANGER_BUTTON_CONTROL_COLOR, advancedTab, std::bind(&UiController::resetSettings, this, _1, _2));

  m_uptimeControl = ESPUI.addControl(ControlType::Label, "uptime [s]", "0", VIEW_CONTROL_COLOR, debugTab);
  m_heapControl = ESPUI.addControl(ControlType::Label, "free heap [kB]", "0", VIEW_CONTROL_COLOR, debugTab);
  m_distance = ESPUI.addControl(ControlType::Label, "distance [cm]", "0", VIEW_CONTROL_COLOR, debugTab);
  ESPUI.addControl(ControlType::Label, "build time", BUILD_TIME, VIEW_CONTROL_COLOR, debugTab);
  ESPUI.addControl(ControlType::Label, "version", GIT_TAG, VIEW_CONTROL_COLOR, debugTab);
  ESPUI.addControl(ControlType::Label, "commit", GIT_COMMIT, VIEW_CONTROL_COLOR, debugTab);
  ESPUI.addControl(ControlType::Button, "restart", "run", DANGER_BUTTON_CONTROL_COLOR, debugTab, std::bind(&UiController::reboot, this, _1, _2));

  ESPUI.beginLITTLEFS(HOSTNAME);
  updateManualPourButton();
}

UiController::~UiController() = default;

void UiController::loop(const std::chrono::milliseconds& now) {
  if (m_lastPrintStatusTime + UI_DEBUG_PRINT_INTERVAL <= now) {
    m_lastPrintStatusTime = now;
    if (m_isDebugTab) {
      ESPUI.updateNumber(m_uptimeControl, std::chrono::duration_cast<std::chrono::seconds>(now).count());
      ESPUI.updateNumber(m_heapControl, ESP.getFreeHeap() / 1024);
      ESPUI.updateNumber(m_distance, m_getDistanceCallback());
    }
  }
}

void UiController::updateManualPourButton() {
  log(LABEL, "update manual pour: %d", m_settings.m_mode == Mode::Manual);
  ESPUI.setEnabled(m_manualPourButton, m_settings.m_mode == Mode::Manual);
}

void UiController::saveSettings() {
  log(LABEL, "save settings");
  m_settings.save();
  m_updateSettingsCallback();
  updateManualPourButton();
}

void UiController::manualPour(Control* sender, int type) {
  if (type != B_UP) {
    return;
  }
  log(LABEL, "manual pour");
  m_manualPourCallback();
}

void UiController::resetSettings(Control* sender, int type) {
  if (type != B_UP) {
    return;
  }
  log(LABEL, "reset settings");
  m_settings.reset();
  m_updateSettingsCallback();
  for (const auto& callback : m_readSettingsCallbacks) {
    callback();
  }
  updateManualPourButton();
}

void UiController::reboot(Control* sender, int type) {
  if (type != B_UP) {
    return;
  }
  log(LABEL, "reboot");
  m_rebootTimer.once(1.0, []() { ESP.restart(); });
}
