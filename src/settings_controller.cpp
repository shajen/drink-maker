#include <EEPROM.h>
#include <logger.h>
#include <settings_controller.h>

constexpr auto SIZE = 1024;
constexpr auto ID = 12345678;

void SettingsController::setup() {
  EEPROM.begin(SIZE);
  if (load(Type::Id, 0) != ID) {
    factoryReset();
  }
}

int SettingsController::load(Type type, int defaultValue) {
  const int* p = reinterpret_cast<const int*>(&EEPROM.getConstDataPtr()[static_cast<int>(type) * 4]);
  return *p != 0 ? *p : defaultValue;
}

void SettingsController::save(Type type, int value) {
  int* p = reinterpret_cast<int*>(&EEPROM.getDataPtr()[static_cast<int>(type) * 4]);
  *p = value;
  EEPROM.commit();
}

void SettingsController::factoryReset() {
  log("settings", "factory reset");
  for (int i = 0; i < SIZE; ++i) {
    EEPROM.write(i, 0);
  }
  save(Type::Id, ID);
}