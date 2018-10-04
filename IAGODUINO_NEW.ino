/*
Will move both motors to the home position (syringes full) then oscillate them
from full to empty, one motor at a time
both motors can also be moved together, but this limits the maximum speed of each motor

Reference for AccelStepper library:
http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html
*/

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


void setup() {
  Serial.begin(9600);

  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, LOW);

  pinMode(LIMIT_X, INPUT_PULLUP);
  pinMode(LIMIT_Y, INPUT_PULLUP);
  
  stepperX.setMaxSpeed(MAX_SPEED);
  stepperX.setAcceleration(ACCELERATION);
  stepperY.setMaxSpeed(MAX_SPEED);
  stepperY.setAcceleration(ACCELERATION);
  stepperX.setPinsInverted(1,0,0);
  stepperY.setPinsInverted(1,0,0);

  Serial.println("homing...");

  // homing
  while(digitalRead(LIMIT_X) > 0) {
    stepperX.move(-750);
    stepperX.run();
  }
  while(digitalRead(LIMIT_Y) > 0) {
    stepperY.move(-750);
    stepperY.run();
  }
  stepperX.setCurrentPosition(0);
  stepperY.setCurrentPosition(0);

  Serial.println("ready");

  stepperX.moveTo(MAX_TRAVEL);

}


void loop() {

  while(stepperX.distanceToGo() != 0) {
    stepperX.run();
  }

  if(stepperY.currentPosition() > 0) {
    stepperY.moveTo(0);
  } else {
    stepperY.moveTo(MAX_TRAVEL);
  }

  while(stepperY.distanceToGo() != 0) {
    stepperY.run();
  }

  if(stepperX.currentPosition() > 0) {
    stepperX.moveTo(0);
  } else {
    stepperX.moveTo(MAX_TRAVEL);
  }
  delay(100);


}
