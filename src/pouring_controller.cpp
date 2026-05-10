#include <config.h>
#include <logger.h>
#include <pouring_controller.h>

#include <cstring>

using namespace std::chrono_literals;

constexpr auto LABEL = "pouring";

PouringController::PouringController(const Settings& settings, const BatteryController& batteryController, Display& display, GlassDetector& glassDetector, PumpController& pumpController, int& counter)
    : m_settings(settings), m_batteryController(batteryController), m_display(display), m_glassDetector(glassDetector), m_pumpController(pumpController), m_counter(counter), m_startPouringTime(0) {
  log(LABEL, "init");
  m_display.setState(Display::State::Pouring);

  m_glassDetector.setDetectionDistance(m_settings.m_distance);
  m_glassDetector.setGlassDetectionDelay(m_settings.m_glassDetectionDelay);
  m_glassDetector.setGlassDisappearDelay(m_settings.m_glassDisappearDelay);

  updateDisplay(0ms, 0.0f);
}

PouringController::~PouringController() = default;

void PouringController::loop(const std::chrono::milliseconds& now) {
  if (m_settings.m_mode == Mode::Auto) {
    loopAuto(now);
  } else if (m_settings.m_mode == Mode::Manual) {
    loopManual(now);
  }
}

void PouringController::loopAuto(const std::chrono::milliseconds& now) {
  const auto status = m_glassDetector.status(now);
  if (status == GlassDetector::Status::Appeared) {
    log(LABEL, "glass detected");
    m_counter++;
    m_startPouringTime = now;
    updateDisplay(0ms, 0.0f);
    m_pumpController.setEnabled(true);
  } else if (status == GlassDetector::Status::Disappeared) {
    log(LABEL, "glass gone");
    updateDisplay(0ms, 0.0f);
    m_pumpController.setEnabled(false);
  } else if (status == GlassDetector::Status::Detected && m_startPouringTime != 0ms) {
    const auto pouringTime = m_settings.m_capacity * PUMP_FACTOR;
    const auto pouringFactor = std::min(1.0f, (now - m_startPouringTime).count() / static_cast<float>(pouringTime.count()));
    const auto remainingTime = pouringTime - (now - m_startPouringTime);
    updateDisplay(remainingTime, pouringFactor);
    if (m_startPouringTime + pouringTime <= now) {
      log(LABEL, "glass full");
      m_pumpController.setEnabled(false);
      m_startPouringTime = 0ms;
    }
  } else {
    updateDisplay(0ms, 0.0f);
  }
}

void PouringController::loopManual(const std::chrono::milliseconds& now) {
  if (m_startPouringTime != 0ms) {
    const auto pouringTime = m_settings.m_capacity * PUMP_FACTOR;
    const auto pouringFactor = std::min(1.0f, (now - m_startPouringTime).count() / static_cast<float>(pouringTime.count()));
    const auto remainingTime = pouringTime - (now - m_startPouringTime);
    updateDisplay(remainingTime, pouringFactor);
    if (m_startPouringTime + pouringTime <= now) {
      log(LABEL, "glass full");
      m_pumpController.setEnabled(false);
      m_startPouringTime = 0ms;
    }
  } else {
    updateDisplay(0ms, 0.0f);
  }
}

void PouringController::startManualPouring() {
  if (m_settings.m_mode != Mode::Manual) {
    return;
  }
  log(LABEL, "start manual pouring");
  m_counter++;
  m_startPouringTime = std::chrono::milliseconds(millis());
  updateDisplay(0ms, 0.0f);
  m_pumpController.setEnabled(true);
}

void PouringController::updateDisplay(const std::chrono::milliseconds remainingTime, const float factor) {
  std::ignore = remainingTime;
  m_display.setPouringData(m_counter, factor);
}
