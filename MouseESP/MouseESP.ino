#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_NeoPixel.h>

String colores[] = {"#000000", "#FF0000", "#00FF00", "#0000FF", "#FFFF00", "#FF00FF", "#00FFFF"};

BleMouse bleMouse;
MPU6050 accelgyro;

const int sensor = 34;

#define PIN 19 
#define NUMPIXELS 16

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);

    Wire.begin();
    Wire.setClock(400000);

    pixels.setBrightness(20);
    pixels.begin();

    // Inicializar el MPU6050
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    accelgyro.setXGyroOffset(0);
    accelgyro.setYGyroOffset(66);
    accelgyro.setZGyroOffset(65);

    Serial.println("Starting BLE work!");
    bleMouse.begin();
}

int x;
int y;

void loop() {

    if (bleMouse.isConnected()) {
        int16_t gx, gy, gz;
        accelgyro.getRotation(&gx, &gy, &gz);

        x = gz / 256;
        y = gx / 256;    
    }
    bleMouse.move(-x, -y);
    Serial.print(x);
    Serial.println(y);
}