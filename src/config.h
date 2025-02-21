#pragma once

#include <chrono>

using namespace std::chrono_literals;

constexpr auto DEFAULT_SETTINGS_FILE = "/default_settings.json";
constexpr auto USER_SETTINGS_FILE = "/user_settings.json";

constexpr auto HOSTNAME = "vodka";
constexpr auto SSID = "Vodka";
constexpr auto PASSWORD = "12345678";
constexpr auto OTA_PASSWORD = "12345678";
constexpr auto SPLASH_SCREEN_TIME = 2s;

constexpr auto UART_BAUDRATE = 74880;
constexpr auto SDA_PIN = 4;
constexpr auto SCL_PIN = 5;

constexpr auto LCD_ADDRESS = 0x3C;
constexpr auto LCD_WIDTH = 128;
constexpr auto LCD_HEIGHT = 64;
constexpr auto LCD_HEIGHT_STATUS = 16;
constexpr auto LCD_WIDTH_STATUS = 16;

constexpr auto LOX_ADDRESS = 0x29;

constexpr auto WS_LED_PIN = 14;
constexpr auto WS_LEDS_COUNT = 12;

constexpr auto PUMP_PIN = 12;
constexpr auto PUMP_FACTOR = 300ms;  // time need to pour 1 ml

constexpr auto SPLASH_UPDATE_INTERVAL = 50ms;
constexpr auto STATUS_PRINT_INTERVAL = 1000ms;
constexpr auto UI_DEBUG_PRINT_INTERVAL = 100ms;
constexpr auto FULL_ANIMATION_TIME = 500ms;
