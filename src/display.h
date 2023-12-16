#pragma once

#include <Adafruit_SSD1306.h>
#include <helpers.h>
#include <thread.h>

#include <chrono>
#include <string>

class Display : public Thread {
 public:
  enum class State { Splash, Pouring, Debug };

  Display();
  ~Display();

  void loop(const std::chrono::milliseconds& now) override;
  void setState(const State state);
  void setPouringData(const std::string& line1, const std::string& line2, float progress);
  void setEditorData(const std::string& editorMode, const std::string& editorLabel, const std::string& editorValue);

 private:
  void showStatus();

  Adafruit_SSD1306 m_display;
  State m_state;
  bool m_needUpdate;
  int m_progressHeight;
  std::string m_pouringLine1;
  std::string m_pouringLine2;
  std::string m_editorMode;
  std::string m_editorLabel;
  std::string m_editorValue;
};
