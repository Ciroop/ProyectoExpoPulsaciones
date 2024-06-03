#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <BleMouse.h>

MPU6050 mpu;
BleMouse bleMouse;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
  
  Serial.println("MPU6050 connection successful");

  bleMouse.begin();
}

void loop() {
  if (bleMouse.isConnected()) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    // Escala de los datos de aceleración para el movimiento del ratón
    int mouseX = ax / 1000; // Ajusta la escala según sea necesario
    int mouseY = ay / 1000; // Ajusta la escala según sea necesario

    // Mover el ratón
    bleMouse.move(mouseX, mouseY);

    Serial.print("Mouse move: X=");
    Serial.print(mouseX);
    Serial.print(" Y=");
    Serial.println(mouseY);

    delay(100); // Ajusta el retardo según sea necesario para suavizar el movimiento
  }
}




/*
#include <Arduino.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Mouse.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;
int button1 = 6;
int button2 = 7;
int buttonState1 = 0; 
int buttonState2 = 0; 

void setup() {

  Serial.begin(9600);
  Wire.begin();
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  mpu.initialize();
  if (!mpu.testConnection()) { while (1); }
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  vx = (gx+15)/150;  
  vy = -(gz-100)/150;

  Serial.print("gx = ");
  Serial.print(gx);
  Serial.print(" | gz = ");
  Serial.print(gz);
  
  Serial.print("        | X = ");
  Serial.print(vx);
  Serial.print(" | Y = ");
  Serial.println(vy);
  
  Mouse.move(vx, vy);
  
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  
 if (buttonState1 == HIGH) {
    Mouse.press(MOUSE_LEFT);
    delay(100);
    Mouse.release(MOUSE_LEFT);
    delay(200);
  } 
  else if(buttonState2 == HIGH) {
    Mouse.press(MOUSE_RIGHT);
    delay(100);
    Mouse.release(MOUSE_RIGHT);
    delay(200);
  }
  delay(80);
}
*/