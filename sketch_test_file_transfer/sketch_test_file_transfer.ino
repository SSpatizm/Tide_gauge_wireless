// Starter sketch to initiate a simulate a file transfer.
// fileData variable should be replaced with an SD card read if the SD is wired to the xiao for data transfer. 


#include <ArduinoBLE.h>

BLEService dataService("12345678-1234-5678-1234-56789abcdef0");

BLEStringCharacteristic fileListChar("12345678-1234-5678-1234-56789abcdef1", BLERead, 100);
BLEStringCharacteristic fileRequestChar("12345678-1234-5678-1234-56789abcdef2", BLEWrite, 50);
BLECharacteristic fileChunkChar("12345678-1234-5678-1234-56789abcdef3", BLERead | BLENotify, 100);

String requestedFile = "";
String fileData = "Example content from file...\nSecond line...\nThird line...\n";
int offset = 0;

void setup() {
Serial.begin(115200);
while (!Serial);

BLE.begin();
BLE.setLocalName("TideGauge");
BLE.setAdvertisedService(dataService);

dataService.addCharacteristic(fileListChar);
dataService.addCharacteristic(fileRequestChar);
dataService.addCharacteristic(fileChunkChar);

BLE.addService(dataService);

fileListChar.writeValue("data01.txt,data02.txt");

BLE.advertise();
Serial.println("BLE device started, waiting for connections...");
}

void loop() {
BLEDevice central = BLE.central();

if (central) {
 Serial.print("Connected to central: ");
 Serial.println(central.address());

 while (central.connected()) {
   if (fileRequestChar.written()) {
     requestedFile = fileRequestChar.value();
     offset = 0;
     Serial.print("File requested: ");
     Serial.println(requestedFile);
   }

   // Send next chunk if there is still data
   if (requestedFile.length() > 0 && offset < fileData.length()) {
     String chunk = fileData.substring(offset, offset + 100);
     fileChunkChar.writeValue((uint8_t*)chunk.c_str(), chunk.length());
     offset += 100;
     delay(300);  // pacing interval
   }
 }

 Serial.println("Disconnected from central");
}
}