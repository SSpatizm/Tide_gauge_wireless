# Tide_gauge_wireless

Firmware for wireless RF integration of the Water Sense Project.

## Goals ##

- Add bluetooth capability to allow remote data downloads or uploads to the tide gauge.
- Reduce current draw through various means, such as scheduling on - off times for receiving.

## Hardware Requirements

- Seeed Studio XIAO nRF52840 (https://wiki.seeedstudio.com/XIAO_BLE/)
- USB Type-C cable
- SD card module (wired to XIAO or another MCU)
- Smartphone with Bluetooth (iOS or Android)

## Software Requirements

- Arduino IDE or PlatformIO
- **Seeed nRF52 Boards Library**
- **ArduinoBLE** library (installed by default with board package)
- nRF Connect for Mobile (Android)(https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp) or LightBlue (iOS)(https://apps.apple.com/us/app/lightblue/id557428110). There is an nRF connect for desktop but it has not been tested yet. 

## Board Setup

Goto File -> Preferences -> Add this URL to the board managers URL: https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json 
Goto Tools -> Board -> Seeed XIAO nRF52840

### *NOTE: THE REST WILL REQUIRE A CONNECTED XIAO BOARD WITH USB C* ###

## Upload BLE Firmware

- To test, upload the LED test sketch_wifi_ex.ino to test the chip.
- Once it is confirmed working, move on to the sketch_test_file_transfer.ino to test data transfer over bluetooth.

## How to Connect ##

1. Open nRF Connect or LightBlue
2. Search for "TideGauge" (or possible could be named "Arudino") and connect.
3. Check the access characteristics for names such as File List, File Request, and File Chunk, which can be used to show availible file names, request specific files by name, or start receiving files, respectively. 
