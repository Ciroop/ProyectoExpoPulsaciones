/**
 * This example turns the ESP32 into a Bluetooth LE mouse that continuously moves the mouse.
 */
#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"

BleMouse bleMouse;
MPU6050 accelgyro;

void setup() {
  Serial.begin(115200);
  

  Wire.begin();
  Wire.setClock(400000);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  accelgyro.setXGyroOffset(-55);
  accelgyro.setYGyroOffset(66);
  accelgyro.setZGyroOffset(65);

  Serial.println("Starting BLE work!");
  bleMouse.begin();
}

void loop() {
  if(bleMouse.isConnected()) {
    int16_t gx, gy, gz;
    accelgyro.getRotation(&gx, &gy, &gz);

    int x = gz/256;
    int y = gy/256;
    Serial.print(x);
    Serial.print("  ");
    Serial.println(y);
    bleMouse.move(-x, -y);
    delay(10);  
  }
}
