# Introduction

The project is to build a device for automatically pouring vodka into glasses that move on wooden toy tracks.

# Presentation

TODO

# Quickstart

## Needed hardware

- `ESP8266` board with Li-Ion 18650
- `SSD1306` OLED 0.96" `I2C` 128x64
- `VL53L0X` time of flight distance sensor `I2C`
- 5 V relay module
- 5 V mini pump
- 5 V `WS2812` 12 led circle
- two push buttons
- silicone tube

## Pinout

| Description | ESP8266 PIN | ESP8266 PIN label |
| -- | --- | --- |
| I2C SCL | GPIO5 | D1 |
| I2C SDA | GPIO4 | D2 |
| Relay | GPIO2 | D4 |
| WS2812 | GPIO14 | D5 |
| Button 1 | GPIO0 | D3 |
| Button 2 | GPIO12 | D6 |

## Flash

```
platformio run -t upload
```

## Settings

During splash screen press any button to enter settings mode.

Single click to increase or decrease current value.

Long click to change other settings. 

# Contributing

In general don't be afraid to send pull request. Use the "fork-and-pull" Git workflow.

1. **Fork** the repo
2. **Clone** the project to your own machine
3. **Commit** changes to your own branch
4. **Push** your work back up to your fork
5. Submit a **Pull request** so that we can review your changes

NOTE: Be sure to merge the **latest** from **upstream** before making a pull request!

# Donations

If you enjoy this project and want to thanks, please use follow link:

[<img src="images/paypal.jpg" width="100">](https://www.paypal.com/donate/?hosted_button_id=6JQ963AU688QN)
[<img src="images/revolut.jpg" width="100">](https://revolut.me/borysm2b)
<img src="images/btc.png" width="100">

BTC address: 18UDYg9mu26K2E3U479eMvMZXPDpswR7Jn

# License

[![License](https://img.shields.io/:license-GPLv3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl.html)

- *[GPLv3 license](https://www.gnu.org/licenses/gpl.html)*
