#include "taskBLE.h"
#include <ArduinoBLE.h>

// BLE Service and Characteristics
BLEService dataService("12345678-1234-5678-1234-56789abcdef0");
BLEStringCharacteristic fileListChar("12345678-1234-5678-1234-56789abcdef1", BLERead, 100);
BLEStringCharacteristic fileRequestChar("12345678-1234-5678-1234-56789abcdef2", BLEWrite, 50);
BLECharacteristic fileChunkChar("12345678-1234-5678-1234-56789abcdef3", BLERead | BLENotify, 100);

// State control
enum BLEState {
  BLE_ADVERTISE,
  BLE_CONNECTED,
  BLE_TRANSFER
};

BLEState bleState = BLE_ADVERTISE;

String requestedFile = "";
String fileData = "Mock file contents for testing...\nLine 2...\nLine 3...";
int offset = 0;

void taskBLE(void* params)
{
  BLE.begin();
  BLE.setLocalName("TideGauge");
  BLE.setAdvertisedService(dataService);

  dataService.addCharacteristic(fileListChar);
  dataService.addCharacteristic(fileRequestChar);
  dataService.addCharacteristic(fileChunkChar);

  BLE.addService(dataService);
  fileListChar.writeValue("data01.txt,data02.txt");  // hardcoded for now

  BLE.advertise();
  Serial.println("[BLE] Advertising...");

  for (;;) {
    switch (bleState) {
      case BLE_ADVERTISE: {
        BLEDevice central = BLE.central();
        if (central) {
          Serial.print("[BLE] Connected to: ");
          Serial.println(central.address());
          bleState = BLE_CONNECTED;
        }
        break;
      }

      case BLE_CONNECTED: {
        if (BLE.connected()) {
          if (fileRequestChar.written()) {
            requestedFile = fileRequestChar.value();
            offset = 0;
            Serial.print("[BLE] File requested: ");
            Serial.println(requestedFile);
            bleState = BLE_TRANSFER;
          }
        } else {
          bleState = BLE_ADVERTISE;
          BLE.advertise();
        }
        break;
      }

      case BLE_TRANSFER: {
        if (BLE.connected()) {
          if (offset < fileData.length()) {
            String chunk = fileData.substring(offset, offset + 100);
            fileChunkChar.writeValue((uint8_t*)chunk.c_str(), chunk.length());
            offset += 100;
            delay(300);  // throttle notifications
          } else {
            Serial.println("[BLE] Transfer complete.");
            bleState = BLE_CONNECTED;
          }
        } else {
          bleState = BLE_ADVERTISE;
          BLE.advertise();
        }
        break;
      }
    }

    delay(50);  // delay inbetween. Will need to change later for power consumption reasons
  }
}
