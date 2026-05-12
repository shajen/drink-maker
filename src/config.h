#pragma once

#include <chrono>

using namespace std::chrono_literals;

constexpr auto DEFAULT_SETTINGS_FILE = "/default_settings.json";
constexpr auto USER_SETTINGS_FILE = "/user_settings.json";

constexpr auto HOSTNAME = "vodka";
constexpr auto SSID = "Vodka";
constexpr auto PASSWORD = "12345678";
constexpr auto OTA_PASSWORD = "12345678";

constexpr auto UART_BAUDRATE = 115200;
constexpr auto SDA_PIN = 8;
constexpr auto SCL_PIN = 9;

constexpr auto LCD_WIDTH = 320;
constexpr auto LCD_HEIGHT = 240;
constexpr auto LCD_CS = 5;
constexpr auto LCD_DC = 6;
constexpr auto LCD_MOSI = 10;
constexpr auto LCD_SCLK = 1;
constexpr auto LCD_RST = 7;

constexpr auto LOX_ADDRESS = 0x29;
constexpr auto LOX_MEASURE_TIME = 500ms;

constexpr auto PUMP_PIN = 0;
constexpr auto PUMP_FACTOR = 300ms;  // time need to pour 1 ml

constexpr auto SPLASH_SCREEN_TIME = std::chrono::seconds(3);
constexpr auto STATUS_PRINT_INTERVAL = 1000ms;
constexpr auto BATTERY_READ_INTERVAL = 1000ms;
constexpr auto UI_DEBUG_PRINT_INTERVAL = 100ms;

constexpr auto SHORT_STRING_SIZE = 20;
constexpr auto LONG_STRING_SIZE = 500;
constexpr auto JSON_SIZE = 500;
