/**
 * This example turns the ESP32 into a Bluetooth LE mouse that continuously moves the mouse.
 */
#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"

BleMouse bleMouse;
MPU6050 accelgyro;
const int boton = 23; 

int estaon = LOW;

void setup() {
  Serial.begin(115200);
  
  pinMode(boton, INPUT);
  

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
    int y = gx/256;

    Serial.print(gz)
    Serial.println(gx);
    
/*
    Serial.print(x);
    Serial.print("  ");
    Serial.println(y);*/

    bleMouse.move(-x, -y);

    Serial.println(digitalRead(boton));

    if (digitalRead(boton) == HIGH) {
      Serial.println("Left click");
      bleMouse.click(MOUSE_LEFT);      
    }
  delay(10);  
  }
}
