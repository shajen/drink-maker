#pragma once

#include <thread.h>

#include <chrono>

class WifiController : public Thread {
 public:
  WifiController();
  ~WifiController();

  void loop(const std::chrono::milliseconds& now) override;
};
