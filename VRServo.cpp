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
extern unsigned long moving_time_limit;
extern unsigned long min_moving_time;

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
        *tilt_angle = pos;
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

void motorspeedwrite(int motortype, int pos, long duration, int* pan_angle, int* tilt_angle) {
  if (motortype == STEPPER) {
    // Can't exactly control the speed of a stepper motor the normal way, and it is slow as is. Thus, handle it regularly
    motorwrite(STEPPER, pos, NULL, NULL);
    return;
  }

  // If the input position or other parameters are incorrect, handle them here
  if (!validmotor(motortype) || !validpos(pos)) {
    return;
  }

  // If the direct user or API user decides to make the duration abnormally long or abnormally short, try to correct and put hard limits here
  if (duration > moving_time_limit) {
    duration = moving_time_limit;
  } else if (duration < min_moving_time) {
    duration = min_moving_time;
  } else {
    duration = (unsigned long) duration; // Cast to prevent potential issues
  }

  // Start a timer, keep track of the time it takes to go some amount of angle
  unsigned long start_time = millis();
  unsigned long progress = 0;
  int start_pan = *pan_angle;
  int start_tilt = *tilt_angle;
  while ((progress < duration) && !Serial.available()) {
    /* For the servos to keep trying to move like this, the process cannot take longer than 5 seconds and there should not be any other input
     * awaiting read. This is so that if the user wants to move the servo slowly but then changed their mind, they should be able to skip the 
     * original process and go on to the new process.
     */
    
    long angle;
    switch (motortype) {
      case SERVO_PAN:
        angle = map(progress, 0, duration, start_pan, pos);
        servoPan.write(angle);
        *pan_angle = angle;
        break;

      case SERVO_TILT:
        angle = map(progress, 0, duration, start_tilt, pos);
        servoTilt.write(angle);
        *tilt_angle = angle;
        break;
    }
    
    // Update progress for next iteration of while loop
    progress = millis() - start_time;
  }

  // Print progress to debug
  Serial.print("Servo has moved to position: ");
  Serial.println(pos);
  Serial.print("Duration in ms: ");
  Serial.println(progress);
}

void motorcswrite(int motortype, int pos, int speed, int* pan_angle, int* tilt_angle) {
  // Use motorspeedwrite to make this a wrapper function rather than having to rewrite this whole algorithm from scratch

  // First, we calculate the time required to move at this speed using simple math
  long duration = 1;
  switch (motortype) {
    case SERVO_PAN:
      // use the current pan_angle
      duration = (long) (abs(pos - *pan_angle) / speed);
      break;

    case SERVO_TILT:
      // use the current tilt_angle
      duration = (long) (abs(pos = *tilt_angle) / speed);
      break;
  }
  
}

bool validmotor(int motor) {
  return (motor < 3) && (motor >= 0);
}

bool validpos(int pos) {
  return (pos <= 180) && (pos >= 0);
}

#endif