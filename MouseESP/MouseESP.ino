/**
 * This example turns the ESP32 into a Bluetooth LE mouse that continuously moves the mouse.
 */
#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "DFRobot_Heartrate.h"

BleMouse bleMouse;
MPU6050 accelgyro;
const int boton = 23;
const int sensor = 34;

int estaon = LOW;
DFRobot_Heartrate heartrate(DIGITAL_MODE);

void setup() {
  Serial.begin(115200);
  
  pinMode(boton, INPUT_PULLUP);
  

  Wire.begin();
  Wire.setClock(400000);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  accelgyro.setXGyroOffset(0);
  accelgyro.setYGyroOffset(66);
  accelgyro.setZGyroOffset(65);

  Serial.println("Starting BLE work!");
  bleMouse.begin();
}

void loop() {
/*
  uint8_t rateValue;
  heartrate.getValue(34);   
  rateValue = heartrate.getRate();   // Get heart rate value 
  if(rateValue)  {
    Serial.println(rateValue);
  }
  delay(20);
*/

  if(bleMouse.isConnected()) {
    
    int16_t gx, gy, gz;
    accelgyro.getRotation(&gx, &gy, &gz);

    int x = gz/256;
    int y = gx/256;    

    Serial.print(x);
    Serial.print("  ");
    Serial.println(y);

    bleMouse.move(-x, -y);

    //  Serial.println(digitalRead(boton));

    if (digitalRead(boton) == LOW) {
      Serial.println("Left click");
      
      bleMouse.click(MOUSE_LEFT);      
    }
  delay(10);  
  
  }
  
}
