#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"

BleMouse bleMouse;
MPU6050 accelgyro;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  Wire.begin();
  Wire.setClock(400000);

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  accelgyro.setXGyroOffset(100);
  accelgyro.setYGyroOffset(66);
  accelgyro.setZGyroOffset(65);

  bleMouse.begin();
}

int x, y;
const int threshold = 2; // Umbral para ignorar movimientos pequeños
const int smoothingFactor = 10; // Factor de suavizado

void loop() {
  int16_t gx, gy, gz;
  accelgyro.getRotation(&gx, &gy, &gz);

  // Suavizar las lecturas
  x = (gz / 256) / smoothingFactor;
  y = (gx / 256) / smoothingFactor;

  // Aplicar el umbral
  if (abs(x) < threshold) x = 0;
  if (abs(y) < threshold) y = 0;

  Serial.print(x);
  Serial.print("----");
  Serial.println(y);
  
  delay(50); // Reducir el delay para una respuesta más rápida

  if (bleMouse.isConnected()) {
    bleMouse.move(-x, -y);
  }
}


 
