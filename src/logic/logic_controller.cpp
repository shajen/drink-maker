#include <logic/logic_controller.h>

using namespace std::chrono_literals;

LogicController::LogicController(StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController)
    : m_statusController(statusController), m_display(display), m_glassDetector(glassDetector), m_ledController(ledController), m_pumpController(pumpController) {
  m_capacity = SettingsController::load(SettingsController::Type::Capacity, 30);
  m_mode = static_cast<Mode>(SettingsController::load(SettingsController::Type::Mode, 1));
  m_distance = SettingsController::load(SettingsController::Type::Distance, 10);
  m_brightness = SettingsController::load(SettingsController::Type::Brightness, 100);
  m_glassDetectionDelay = std::chrono::milliseconds(SettingsController::load(SettingsController::Type::GlassDetectionDelay, 3000));
  m_glassDisappearDelay = std::chrono::milliseconds(SettingsController::load(SettingsController::Type::GlassDisappearDelay, 50));

  m_ledController.setBrightness(m_brightness);
}

LogicController::~LogicController() = default;

void LogicController::onButton1Click() {}
void LogicController::onButton2Click() {}
void LogicController::onButton1LongClick() {}
void LogicController::onButton2LongClick() {}