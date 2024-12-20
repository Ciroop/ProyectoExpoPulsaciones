#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <WebSocketsServer.h>

String colores[] = {"#010101", "#FF0000", "#00FF00", "#0000FF", "#FFFF00", "#FF00FF", "#00FFFF"};

const char* ssid = "grafiti";     // Reemplaza con el nombre de tu red Wi-Fi
const char* password = "cirociro"; // Reemplaza con la contraseña de tu red


String color;


WebSocketsServer webSocket = WebSocketsServer(81); // Puerto 81

BleMouse bleMouse;
MPU6050 accelgyro;

int counter = 0; // Contador
char direction = ' '; // Direccion
long valorRel=0;

const int buttonPin = 25; 

#define PIN 19 
#define NUMPIXELS 19

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

bool buttonHeld = false;


void hexToRGB(String hexString, uint8_t &r, uint8_t &g, uint8_t &b) {
  if (hexString.startsWith("#")) {
    hexString = hexString.substring(1); // Remover el "#"
  }
  long number = (long) strtol(hexString.c_str(), NULL, 16); // Convertir a long
  r = (number >> 16) & 0xFF; // Extraer el valor rojo
  g = (number >> 8) & 0xFF;  // Extraer el valor verde
  b = number & 0xFF;         // Extraer el valor azul
}


void setup() {
    Serial.begin(115200);
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(buttonPin, INPUT_PULLUP); 

    // Leer el estado inicial del CLK
    lastStateCLK = digitalRead(CLK);
    
    // Llamar a updateEncoder() cuando un high o un low haya cambiado
    attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, CHANGE);

     // Conectar a WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }
    Serial.println("Conectado a WiFi");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);


    Wire.begin();
    Wire.setClock(400000);

    
    pixels.begin();

    // Inicializar el MPU6050
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    accelgyro.setXGyroOffset(100);
    accelgyro.setYGyroOffset(0);
    accelgyro.setZGyroOffset(65);

    Serial.println("Starting BLE work!");
    bleMouse.begin();
}

int x;
int y;
uint8_t red, green, blue;
const int smoothingFactor = 1.4; // Factor de suavizado
int16_t gx, gy, gz;

void loop() {

  webSocket.loop();
    
    if (updateNeeded) {
        updateNeeded = false; // Resetear el flag
        Serial.print("Direction: ");
        Serial.print(direction);
        Serial.print(" | Counter: ");
        Serial.println(counter);
    }

    
    if (bleMouse.isConnected()) {

    accelgyro.getRotation(&gx, &gy, &gz);

    // Suavizar las lecturas
    x = (gy / 256) / smoothingFactor * -1;
    y = (gx / 256) / smoothingFactor;


    if (x < 0) {
      x = x / 1.05;  // Aplica una suavización adicional para movimientos a la derecha
    }
      
    Serial.print(x);
    Serial.print("----");
    Serial.println(y);
      
    bleMouse.move(-x, -y);
  }
     
 

  hexToRGB(color, red, green, blue); // Convertir el color

  for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();


  // Mantener el click si el botón está presionado
  if (buttonHeld) {
    bleMouse.press(MOUSE_LEFT);
  } else {
    bleMouse.release(MOUSE_LEFT);
  }

  if (digitalRead(buttonPin) == LOW) {
    buttonHeld = true;  // El botón está presionado
  } else {
    buttonHeld = false; // El botón está liberado
  }
  
  delay(5);  
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
      case WStype_TEXT:
          char buffer[7]; // Suponiendo que el color es siempre un código hexadecimal de 7 caracteres (ej. "#FF00FF")if (length < sizeof(buffer)) {
          memcpy(buffer, payload, length); 
          buffer[length] = '\0'; // Agregar el terminador nulo al final     
          color = String(buffer); // Asignar a la variable color        
          Serial.printf("El color recibido es: %s\n", color.c_str());
             
        break;
        case WStype_CONNECTED:
            Serial.printf("Cliente conectado desde: %u\n", num);
            break;
        case WStype_DISCONNECTED:
            Serial.printf("Cliente desconectado: %u\n", num);
            break;
    }
    
  }



/*cppCopiar códigovoid webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_TEXT:
            char buffer[7]; // Suponiendo que el color es siempre un código hexadecimal de 7 caracteres (ej. "#FF00FF")if (length < sizeof(buffer)) {
                memcpy(buffer, payload, length); 
                buffer[length] = '\0'; // Agregar el terminador nulo al final     
                color = String(buffer); // Asignar a la variable color        
                Serial.printf("El color recibido es: %s\n", color.c_str());
            }break;
        case WStype_CONNECTED:
            Serial.printf("Cliente conectado desde: %u\n", num);
            break;
        case WStype_DISCONNECTED:
            Serial.printf("Cliente desconectado: %u\n", num);
            break;
    }
}*/


int current=1;
void updateEncoder() {
    int currentStateCLK = digitalRead(CLK);
    int currentStateDT = digitalRead(DT);

    // Comprobar si el estado del DT es diferente al estado del CLK
    if (currentStateCLK != lastStateCLK) {
        if (currentStateDT != currentStateCLK) {
            counter++;
            direction = 'C';
            bleMouse.move(0,0,1);
        } else {
            counter--;
            direction = 'W';
            bleMouse.move(0,0,-1);
        }
        updateNeeded = true; // Indica que hay una actualización
        if (counter>6) counter=0;
        if (counter<0) counter=6;
    }
    
    lastStateCLK = currentStateCLK; // Guardar el último estado del CLK
}