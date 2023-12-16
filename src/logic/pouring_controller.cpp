#include <logger.h>
#include <logic/pouring_controller.h>

using namespace std::chrono_literals;

constexpr auto FACTOR = 1000ms;
constexpr auto FULL_ANIMATION_TIME = 500ms;

PouringController::PouringController(StatusController& statusController, Display& display, GlassDetector& glassDetector, LedController& ledController, PumpController& pumpController)
    : LogicController(statusController, display, glassDetector, ledController, pumpController), m_counter(0), m_startPouringTime(0) {
  m_display.setState(Display::State::Pouring);

  m_glassDetector.setDetectionDistance(m_distance);
  m_glassDetector.setGlassDetectionDelay(m_glassDetectionDelay);
  m_glassDetector.setGlassDisappearDelay(m_glassDisappearDelay);

  m_ledController.setState(LedController::State::Off);

  updateDisplay(0ms, 0.0f);
}

PouringController::~PouringController() = default;

void PouringController::loop(const std::chrono::milliseconds& now) {
  if (m_mode == Mode::Auto) {
    loopAuto(now);
  } else if (m_mode == Mode::Manual) {
    loopManual(now);
  }
}

void PouringController::onButton1Click() {
  if (m_mode == Mode::Manual) {
    startManualPouring();
  }
}

void PouringController::onButton2Click() {
  if (m_mode == Mode::Manual) {
    startManualPouring();
  }
}

void PouringController::loopAuto(const std::chrono::milliseconds& now) {
  const auto status = m_glassDetector.status(now);
  if (status == GlassDetector::Status::Appeared) {
    log("pouring", "glass detected");
    m_counter++;
    m_startPouringTime = now;
    updateDisplay(0ms, 0.0f);
    m_ledController.setPouringData(0.0f);
    m_ledController.setState(LedController::State::Pouring);
    m_pumpController.setEnabled(true);
  } else if (status == GlassDetector::Status::Disappeared) {
    log("pouring", "glass gone");
    updateDisplay(0ms, 0.0f);
    m_ledController.setState(LedController::State::Off);
    m_pumpController.setEnabled(false);
  } else if (status == GlassDetector::Status::Detected && m_startPouringTime != 0ms) {
    const auto pouringTime = m_capacity * FACTOR;
    const auto pouringFactor = std::min(1.0f, (now - m_startPouringTime).count() / static_cast<float>(pouringTime.count()));
    const auto remainingTime = pouringTime - (now - m_startPouringTime);
    updateDisplay(remainingTime, pouringFactor);
    m_ledController.setPouringData(pouringFactor);
    if (m_startPouringTime + pouringTime <= now) {
      log("pouring", "glass full");
      m_pumpController.setEnabled(false);
      m_ledController.setState(LedController::State::Full);
      m_startPouringTime = 0ms;
    }
  }
}

void PouringController::loopManual(const std::chrono::milliseconds& now) {
  if (m_startPouringTime != 0ms) {
    const auto pouringTime = m_capacity * FACTOR;
    const auto pouringFactor = std::min(1.0f, (now - m_startPouringTime).count() / static_cast<float>(pouringTime.count()));
    const auto remainingTime = pouringTime - (now - m_startPouringTime);
    updateDisplay(remainingTime, pouringFactor);
    m_ledController.setPouringData(pouringFactor);
    if (m_startPouringTime + pouringTime <= now) {
      log("pouring", "glass full");
      m_pumpController.setEnabled(false);
      m_ledController.setState(LedController::State::Full);
      m_startPouringTime = 0ms;
      m_startFullAnimationTime = now;
    }
  }
  if (m_startFullAnimationTime != 0ms && m_startFullAnimationTime + FULL_ANIMATION_TIME <= now) {
    m_ledController.setState(LedController::State::Off);
    updateDisplay(0ms, 0.0f);
    m_startFullAnimationTime = 0ms;
  }
}

void PouringController::startManualPouring() {
  log("pouring", "start manual pouring");
  m_counter++;
  m_startPouringTime = std::chrono::milliseconds(millis());
  updateDisplay(0ms, 0.0f);
  m_ledController.setPouringData(0.0f);
  m_ledController.setState(LedController::State::Pouring);
  m_pumpController.setEnabled(true);
}

void PouringController::updateDisplay(const std::chrono::milliseconds remainingTime, const float factor) {
  char line1[100];
  char line2[100];
  std::ignore = remainingTime;
  sprintf(line1, "%d", m_counter);
  if (m_mode == Mode::Auto) {
    sprintf(line2, "%d ml AUTO", m_capacity);
  } else if (m_mode == Mode::Manual) {
    sprintf(line2, "%d ml RECZNY", m_capacity);
  }
  m_display.setPouringData(line1, line2, factor);
}