#pragma once

#include <Adafruit_NeoPixel.h>
#include <thread.h>

class LedController : public Thread {
 public:
  enum class State { Off, Pouring, Full, Splash };

  LedController();
  ~LedController();

  void loop(const std::chrono::milliseconds& now) override;
  void setState(const State state);
  void setPouringData(const float progress);
  void setBrightness(int brightness);

 private:
  Adafruit_NeoPixel m_ws;
  State m_state;
  bool m_needUpdate;
  float m_progress;
  int m_splashLastLed;
  std::chrono::milliseconds m_splashLastUpdate;
};