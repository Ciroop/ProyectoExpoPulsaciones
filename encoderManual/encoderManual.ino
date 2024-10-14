#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_NeoPixel.h>

String colores[] = {"#000000", "#FF0000", "#00FF00", "#0000FF", "#FFFF00", "#FF00FF", "#00FFFF"};

BleMouse bleMouse;
MPU6050 accelgyro;

 int counter = 0; // Contador
 char direction = ' '; // Direccion


const int sensor = 34;
const int buttonPin = 25; 

#define PIN 2 
#define NUMPIXELS 16

#define CLK 27
#define DT 26

char currentDir[4] = ""; // Usar array de caracteres

int currentStateCLK;
int lastStateCLK;

// Flag para indicar que se necesita imprimir el resultado
bool updateNeeded = false;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastButtonPressTime = 0;
const unsigned long debounceDelay = 200; 

void setup() {
    Serial.begin(115200);
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(buttonPin, INPUT_PULLUP); 

    // Leer el estado inicial del CLK
    lastStateCLK = digitalRead(CLK);
    
    // Llamar a updateEncoder() cuando un high o un low haya cambiado
    attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, FALLING);


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
        Serial.print(direction);
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
    delay(1);
}


int current=1;
int valorRel = 0;
void updateEncoder() {
    int currentStateCLK = digitalRead(CLK);
    int currentStateDT = digitalRead(DT);

    // Comprobar si el estado del DT es diferente al estado del CLK
    if (currentStateCLK != lastStateCLK) {
        if (currentStateDT != currentStateCLK) {
            counter = (counter == 7) ? 0 : counter + 1; // CW
            direction = 'C';
            bleMouse.move(0,0,1);
        } else {
            counter = (counter == 0) ? 7 : counter - 1; // CCW
            direction = 'W';
            bleMouse.move(0,0,-1);
        }
        updateNeeded = true; // Indica que hay una actualización
    }
    
    lastStateCLK = currentStateCLK; // Guardar el último estado del CLK 
    
}

void buttonPressed() {
    unsigned long currentTime = millis();
    
    // Comprobar si el tiempo desde la última presión es mayor que el debounceDelay
    if (currentTime - lastButtonPressTime > debounceDelay) {
      bleMouse.click(MOUSE_LEFT);
      Serial.println("Button Pressed!"); // Imprimir mensaje al presionar el botón
      lastButtonPressTime = currentTime; // Actualizar el tiempo del último pulso
    }
}