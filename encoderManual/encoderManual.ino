#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_NeoPixel.h>

String colores[] = {"#000000", "#FF0000", "#00FF00", "#0000FF", "#FFFF00", "#FF00FF", "#00FFFF"};

BleMouse bleMouse;
MPU6050 accelgyro;

const int sensor = 34;

#define PIN 2 
#define NUMPIXELS 16

#define CLK 27
#define DT 26

int counter = 0; // Contador
char currentDir[4] = ""; // Usar array de caracteres

int currentStateCLK;
int lastStateCLK;

// Flag para indicar que se necesita imprimir el resultado
bool updateNeeded = false;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastDebounceTime = 0; // Último tiempo de cambio de estado
const unsigned long debounceDelay = 50; // Tiempo de debounce (50 ms)

void setup() {
    Serial.begin(115200);
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);

    // Leer el estado inicial del CLK
    lastStateCLK = digitalRead(CLK);
    
    // Llamar a updateEncoder() cuando un high o un low haya cambiado
    attachInterrupt(CLK, updateEncoder, CHANGE);
    attachInterrupt(DT, updateEncoder, CHANGE);

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
    if (updateNeeded) {
        updateNeeded = false; // Resetear el flag
        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter: ");
        Serial.println(counter);
    }

    if (bleMouse.isConnected()) {
        int16_t gx, gy, gz;
        accelgyro.getRotation(&gx, &gy, &gz);

        x = gz / 256;
        y = gx / 256;    
    }
    bleMouse.move(-x, -y);
}

void updateEncoder() {
    unsigned long currentTime = millis();

    // Comprobar el tiempo de debounce
    if (currentTime - lastDebounceTime > debounceDelay) {
        currentStateCLK = digitalRead(CLK);

        // Si el último estado actual del CLK es distinto, ocurrió un pulso 
        if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
            // Si el estado del DT es diferente que el estado del CLK
            if (digitalRead(DT) != currentStateCLK) {
                counter = (counter == 0) ? 7 : counter - 1; 
                strcpy(currentDir, "CCW"); // Cambia la dirección a "CCW"
            } else {
                counter = (counter == 7) ? 0 : counter + 1; 
                strcpy(currentDir, "CW"); // Cambia la dirección a "CW"
            }
            updateNeeded = true; // Indicar que se necesita actualizar el Serial
        }

        lastDebounceTime = currentTime; // Actualizar el tiempo del último cambio
    }
    lastStateCLK = currentStateCLK; // Guardar el último estado del CLK 
}
