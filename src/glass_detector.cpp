#include <config.h>
#include <glass_detector.h>

#include <limits>

GlassDetector::GlassDetector()
    : m_distanceMeter(),
      m_status(GlassDetector::Status::NotDetected),
      m_lastDetectedTime(0),
      m_lastNotDetectedTime(0),
      m_detectionDistance(0),
      m_distance(std::numeric_limits<int>::max()),
      m_glassDetectionDelay(0ms),
      m_glassDisappearDelay(0ms) {
  m_distanceMeter.begin(LOX_ADDRESS);
  m_distanceMeter.startRangeContinuous(std::chrono::duration_cast<std::chrono::milliseconds>(LOX_MEASURE_TIME).count());
}

GlassDetector::~GlassDetector() {}

void GlassDetector::loop(const std::chrono::milliseconds& now) {
  if (m_distanceMeter.isRangeComplete()) {
    m_distance = m_distanceMeter.readRange() / 10.0f;
    if (m_distance <= m_detectionDistance) {
      m_lastDetectedTime = now;
    } else {
      m_lastNotDetectedTime = now;
    }
  }
}

GlassDetector::Status GlassDetector::status(const std::chrono::milliseconds& now) {
  if (m_lastNotDetectedTime + m_glassDetectionDelay <= now) {
    if (m_status == GlassDetector::Status::NotDetected) {
      m_status = GlassDetector::Status::Appeared;
    } else {
      m_status = GlassDetector::Status::Detected;
    }
  } else if (m_lastDetectedTime + m_glassDisappearDelay <= now) {
    if (m_status == GlassDetector::Status::Detected) {
      m_status = GlassDetector::Status::Disappeared;
    } else {
      m_status = GlassDetector::Status::NotDetected;
    }
  }
  return m_status;
}

int GlassDetector::getDistance() const { return m_distance; }

void GlassDetector::setDetectionDistance(int distance) { m_detectionDistance = distance; }

void GlassDetector::setGlassDetectionDelay(std::chrono::milliseconds delay) { m_glassDetectionDelay = delay; }

void GlassDetector::setGlassDisappearDelay(std::chrono::milliseconds delay) { m_glassDisappearDelay = delay; }