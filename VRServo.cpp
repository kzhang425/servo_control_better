#ifndef VRSERVO_H
#define VRSERVO_H

#include <Servo.h>
#include <Stepper.h>
#include "VRServo.h"
#include <Arduino.h>

extern Servo servoPan;
extern Servo servoTilt;
extern Stepper myStepper;
extern int enA;
extern int enB;

void motorwrite(int motortype, int pos) {
  switch (motortype) {
      case SERVO_PAN:
        servoPan.write(pos);
        Serial.print("ServoPan in position\n");
        Serial.println(pos);
        break;

      case SERVO_TILT:
        servoTilt.write(pos);
        Serial.print("ServoTilt in position\n");
        Serial.println(pos);
        break;

      case STEPPER:
        digitalWrite(enA, HIGH);
        digitalWrite(enB, HIGH);

        myStepper.step(pos);
        delay(4000);
        Serial.print("Stepper in position\n");
        Serial.println(pos);
        digitalWrite(enA,LOW);
        digitalWrite(enB, LOW);
        break;
    }
}


#endif