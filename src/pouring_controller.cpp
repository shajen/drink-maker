#include <config.h>
#include <logger.h>
#include <pouring_controller.h>

using namespace std::chrono_literals;

constexpr auto LABEL = "pouring";

PouringController::PouringController(
    const Settings& settings, const BatteryController& batteryController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController, int& counter)
    : m_settings(settings),
      m_batteryController(batteryController),
      m_display(display),
      m_glassDetector(glassDetector),
      m_ledController(ledController),
      m_pumpController(pumpController),
      m_counter(counter),
      m_startPouringTime(0) {
  log(LABEL, "init");
  m_display.setState(Display::State::Pouring);

  m_glassDetector.setDetectionDistance(m_settings.m_distance);
  m_glassDetector.setGlassDetectionDelay(m_settings.m_glassDetectionDelay);
  m_glassDetector.setGlassDisappearDelay(m_settings.m_glassDisappearDelay);

  m_ledController.setState(LedController::State::Off);

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
    m_ledController.setPouringData(0.0f);
    m_ledController.setState(LedController::State::Pouring);
    m_pumpController.setEnabled(true);
  } else if (status == GlassDetector::Status::Disappeared) {
    log(LABEL, "glass gone");
    updateDisplay(0ms, 0.0f);
    m_ledController.setState(LedController::State::Off);
    m_pumpController.setEnabled(false);
  } else if (status == GlassDetector::Status::Detected && m_startPouringTime != 0ms) {
    const auto pouringTime = m_settings.m_capacity * PUMP_FACTOR;
    const auto pouringFactor = std::min(1.0f, (now - m_startPouringTime).count() / static_cast<float>(pouringTime.count()));
    const auto remainingTime = pouringTime - (now - m_startPouringTime);
    updateDisplay(remainingTime, pouringFactor);
    m_ledController.setPouringData(pouringFactor);
    if (m_startPouringTime + pouringTime <= now) {
      log(LABEL, "glass full");
      m_pumpController.setEnabled(false);
      m_ledController.setState(LedController::State::Full);
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
    m_ledController.setPouringData(pouringFactor);
    if (m_startPouringTime + pouringTime <= now) {
      log(LABEL, "glass full");
      m_pumpController.setEnabled(false);
      m_ledController.setState(LedController::State::Full);
      m_startPouringTime = 0ms;
      m_startFullAnimationTime = now;
    }
  } else {
    updateDisplay(0ms, 0.0f);
  }
  if (m_startFullAnimationTime != 0ms && m_startFullAnimationTime + FULL_ANIMATION_TIME <= now) {
    m_ledController.setState(LedController::State::Off);
    updateDisplay(0ms, 0.0f);
    m_startFullAnimationTime = 0ms;
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
  m_ledController.setPouringData(0.0f);
  m_ledController.setState(LedController::State::Pouring);
  m_pumpController.setEnabled(true);
}

void PouringController::updateDisplay(const std::chrono::milliseconds remainingTime, const float factor) {
  std::ignore = remainingTime;
  std::memset(m_line1.data(), 0, sizeof(m_line1));
  std::memset(m_line2.data(), 0, sizeof(m_line2));
  sprintf(m_line1.data(), "%d", m_counter);
  if (m_settings.m_mode == Mode::Auto) {
    sprintf(m_line2.data(), "%dml %d%% A", m_settings.m_capacity, m_batteryController.getPercentage());
  } else if (m_settings.m_mode == Mode::Manual) {
    sprintf(m_line2.data(), "%dml %d%% M", m_settings.m_capacity, m_batteryController.getPercentage());
  }
  m_display.setPouringData(m_line1, m_line2, factor);
}
