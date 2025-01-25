#include <logic/logic_controller.h>

using namespace std::chrono_literals;

LogicController::LogicController(
    const Settings& settings, StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController)
    : m_settings(settings), m_statusController(statusController), m_display(display), m_glassDetector(glassDetector), m_ledController(ledController), m_pumpController(pumpController) {}

LogicController::~LogicController() = default;
