#pragma once

#include <chrono>

class Thread {
 public:
  Thread() = default;
  virtual ~Thread() = default;

  virtual void loop(const std::chrono::milliseconds& now) = 0;
};