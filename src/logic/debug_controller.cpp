#include <logic/debug_controller.h>

using namespace std::chrono_literals;

DebugController::DebugController(StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController)
    : LogicController(statusController, display, glassDetector, ledController, pumpController) {
  m_display.setState(Display::State::Debug);
  m_ledController.setState(LedController::State::Off);
  updateDisplay();
}

DebugController::~DebugController() = default;

void DebugController::loop(const std::chrono::milliseconds& now) { updateDisplay(); }

void DebugController::updateDisplay() {
  char tmp[20];
  sprintf(tmp, "%d cm", m_glassDetector.getDistance());
  m_display.setEditorData("PREVIEW", "DISTANCE", tmp);
}