#include <BleMouse.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "AiEsp32RotaryEncoder.h"
#include <Adafruit_NeoPixel.h>


BleMouse bleMouse;
MPU6050 accelgyro;

const int sensor = 34;
long antEncoder=0;

#define PIN 2 
#define NUMPIXELS 16
#define DELAYVAL 500 

#define ROTARY_ENCODER_A_PIN 19
#define ROTARY_ENCODER_B_PIN 18
#define ROTARY_ENCODER_BUTTON_PIN 17
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int estaon = LOW;

void rotary_onButtonClick()
{
    if (digitalRead(ROTARY_ENCODER_BUTTON_PIN) == LOW) {
      Serial.println("Left click");
      
      bleMouse.click(MOUSE_LEFT);      
    }
}


void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}


void setup() {
  Serial.begin(115200);
    
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);

  Wire.begin();
  Wire.setClock(400000);

  pixels.setBrightness(20);
  pixels.begin();

  bool circleValues = true;
  rotaryEncoder.setBoundaries(0, 100, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

  pinMode(ROTARY_ENCODER_BUTTON_PIN, INPUT_PULLUP);

  rotaryEncoder.setAcceleration(250); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration

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

  if(bleMouse.isConnected()) {
    
    int16_t gx, gy, gz;
    accelgyro.getRotation(&gx, &gy, &gz);

    int x = gz/256;
    int y = gx/256;    

    Serial.print(x);
    Serial.print("  ");
    Serial.println(y);

    long valorRel=0;
    if (rotaryEncoder.encoderChanged())
    {
      Serial.print("Value: ");
      Serial.println(rotaryEncoder.readEncoder());
      valorRel=rotaryEncoder.readEncoder()-antEncoder;
      antEncoder=rotaryEncoder.readEncoder();
    }

    if (rotaryEncoder.isEncoderButtonClicked())
    {
      rotary_onButtonClick();
    }
    Serial.println(valorRel);
    bleMouse.move(-x, -y, valorRel); 
  }

  pixels.clear();

  for(int i=0; i<NUMPIXELS; i++) { 

    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();

    delay(DELAYVAL);
  }

  delay(10);  
}
