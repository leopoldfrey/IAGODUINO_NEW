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
// LED HEADER

// reference for FastLED library:
// https://github.com/FastLED/FastLED/wiki/Controlling-leds
#include "FastLED.h"

#define NUM_LEDS 3
#define DATA_PIN 11

CRGB leds[NUM_LEDS];

int ledInt = 0;
bool fadeIn = true;

void setup() {

  // ----------------------------------
  // MOTOR SETUP
  Serial.begin(9600);

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

  // ----------------------------------
  // LED SETUP
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {

  // ----------------------------------
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

  // ----------------------------------
  // LED LOOP

  if (fadeIn)
    ledInt += 10;
  else
    ledInt -= 10;

  if(ledInt < 0)
  {
    ledInt = 0;
    fadeIn = true;
  } else if (ledInt > 255)
  {
    ledInt = 255;
    fadeIn = false;
  }
  Serial.println(ledInt);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(ledInt, ledInt, ledInt);
  }
  FastLED.show();


  delay(100);


  /*  // fade in
    for(int a=0; a<255; a++) {
      for(int i=0; i<NUM_LEDS; i++) {
        leds[i].setRGB(a, a, a);
      }
      FastLED.show();
      delay(10);
    }

    // fade out
    for(int a=255; a>0; a--) {
      for(int i=0; i<NUM_LEDS; i++) {
        leds[i].setRGB(a, a, a);
      }
      FastLED.show();
      delay(10);
    }
  */

}
