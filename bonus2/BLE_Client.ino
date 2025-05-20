/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

const int output26 = 26;

int scanTime = 1;  //In seconds

float calculateDistance(int rssi, int txPower = -59, float n = 2.7) {
  return pow(10.0, ((float)(txPower - rssi)) / (10 * n));
}

void setup() {
  Serial.begin(115200);

  pinMode(output26, OUTPUT);
  digitalWrite(output26, LOW);

  BLEDevice::init("BLE_Client");
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScan* scanner = BLEDevice::getScan();
  scanner->setActiveScan(true);

  BLEScanResults results = *scanner->start(scanTime, false);

  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice d = results.getDevice(i);
    
    if (d.haveName()) {
      String name = d.getName().c_str();
      
      if (name.equals("BLE_Server")) {
        int rssi = d.getRSSI();
        float estimatedDistance = calculateDistance(rssi);
        Serial.printf("%.2f\n", estimatedDistance);
        if (estimatedDistance > 0.7) {
          Serial.println("Light on");
          digitalWrite(output26, HIGH);
        } else {
          Serial.println("Light off");
          digitalWrite(output26, LOW);
        }
      }
    }
  }
  
  scanner->clearResults();  // delete results fromBLEScan buffer to release memory
}
