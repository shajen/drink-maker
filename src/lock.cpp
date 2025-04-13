#include "lock.h"

#include <Arduino.h>

Lock::Lock() { noInterrupts(); }

Lock::~Lock() { interrupts(); }