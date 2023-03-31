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

void motorwrite(int motortype, int pos, int* pan_angle, int* tilt_angle) {
  bool invalid = (pos < 0) || (pos > 180);
  switch (motortype) {
      case SERVO_PAN:
        if (invalid) {
          Serial.println("Invalid position.");
          return;
        }
        *pan_angle = pos;
        servoPan.write(pos);
        Serial.print("ServoPan in position\n");
        Serial.println(pos);
        break;

      case SERVO_TILT:
        if (invalid) {
          Serial.println("Invalid position.");
          return;
        }
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

void motorspeedwrite(int motortype, int pos, int duration, int* pan_angle, int* tilt_angle) {
  if (motortype == STEPPER) {
    // Can't exactly control the speed of a stepper motor the normal way, and it is slow as is. Thus, handle it regularly
    motorwrite(STEPPER, pos, NULL, NULL);
    return;
  }


}


#endif