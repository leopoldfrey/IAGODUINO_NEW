

  // ----------------------------------
  // MOTOR HEADER

  //Reference for AccelStepper library:
  //http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html
  #include <AccelStepper.h>

  // GRBL shield pinouts
  #define STEP_X 2
  #define STEP_Y 3
  #define DIR_X 5
  #define DIR_Y 6
  #define LIMIT_X 9
  #define LIMIT_Y 10
  #define ENABLE 8

  AccelStepper stepperX(AccelStepper::DRIVER, STEP_X, DIR_X);
  AccelStepper stepperY(AccelStepper::DRIVER, STEP_Y, DIR_Y);

  #define MAX_SPEED 20000
  #define ACCELERATION 6000
  #define MAX_TRAVEL 6100

// ----------------------------------
// BUTTON HEADER
int ledPin = 13;                // choose the pin for the LED
int inputPin = 3;               // Connect sensor to input pin 3

// ----------------------------------
// LED HEADER

// reference for FastLED library:
// https://github.com/FastLED/FastLED/wiki/Controlling-leds
#include "FastLED.h"

#define NUM_LEDS 4
#define DATA_PIN 11

CRGB leds[NUM_LEDS];

// SERIAL HEADER

boolean valComplete = false;
String val = "";
int count = 0;
int prevBut = LOW;

// ----------------------------------
// SETUP

void setup() {
  // BUTTON SETUP
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare pushbutton as input

  // LED SETUP
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // SERIAL SETUP
  Serial.begin (115200);
  delay(1000);

  
    // MOTOR SETUP
    //Serial.begin(9600);

    pinMode(ENABLE, OUTPUT);
    digitalWrite(ENABLE, LOW);

    pinMode(LIMIT_X, INPUT_PULLUP);
    pinMode(LIMIT_Y, INPUT_PULLUP);

    stepperX.setMaxSpeed(MAX_SPEED);
    stepperX.setAcceleration(ACCELERATION);
    stepperY.setMaxSpeed(MAX_SPEED);
    stepperY.setAcceleration(ACCELERATION);
    stepperX.setPinsInverted(1, 0, 0);
    stepperY.setPinsInverted(1, 0, 0);

    Serial.println("homing...");

    // homing
    while (digitalRead(LIMIT_X) > 0) {
    stepperX.move(-750);
    stepperX.run();
    }
    while (digitalRead(LIMIT_Y) > 0) {
    stepperY.move(-750);
    stepperY.run();
    }
    stepperX.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);

    Serial.println("ready");

    stepperX.moveTo(MAX_TRAVEL);
}

// ----------------------------------
// LOOP

void loop() {

  // BUTTON LOOP

  int val = digitalRead(inputPin);  // read input value
  if (prevBut != val)
  {
    printBut(val);
    prevBut = val;
  }

  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin, HIGH); // turn LED ON
  }

  // LED LOOP

  if (valComplete == true)
  {
    printLeds();
    FastLED.show();
    valComplete = false;
    val = "";
  }

  
    // MOTOR LOOP

    while (stepperX.distanceToGo() != 0) {
    stepperX.run();
    }

    if (stepperY.currentPosition() > 0) {
    stepperY.moveTo(0);
    } else {
    stepperY.moveTo(MAX_TRAVEL);
    }

    while (stepperY.distanceToGo() != 0) {
    stepperY.run();
    }

    if (stepperX.currentPosition() > 0) {
    stepperX.moveTo(0);
    } else {
    stepperX.moveTo(MAX_TRAVEL);
    }
      //delay(100);
      //*/
}

// ----------------------------------
// SERIAL EVENT

void serialEvent() {
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar == '(') {
      count = 0;
      val = "";
    } else if (inChar == '_') {
      toLed(count, val.toInt());
      count++;
      val = "";
    } else if (inChar == ')') {
      toLed(count, val.toInt());
      valComplete = true;
      val = "";
    } else {
      val += inChar;
    }
  }
}

// ----------------------------------
// UTILS

void printBut(int val) {
  Serial.print("BUT ");
  if (val == HIGH) {
    Serial.println(1);
  } else {
    Serial.println(0);
  }
}

void printLeds() {
  Serial.print("LED");
  for (int i = 0 ; i < NUM_LEDS ; i++)
  {
    Serial.print(" ");
    Serial.print(leds[i].r);
    Serial.print(" ");
    Serial.print(leds[i].g);
    Serial.print(" ");
    Serial.print(leds[i].b);
  }
  Serial.println();
}

void toLed(int c, int v) {
  int n = (int)(c / 3);
  int rgb = (int)(c % 3);

  switch (rgb) {
    case 0:
      leds[n].r = v;
      break;
    case 1:
      leds[n].g = v;
      break;
    case 2:
      leds[n].b = v;
      break;
  }
}
