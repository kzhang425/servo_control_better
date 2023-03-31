#include <Servo.h>
#include <Stepper.h>
#include "VRServo.h"

// Initialize devices
Servo servoPan;
Servo servoTilt;
// Initialize values
int pan_angle = 0;
int tilt_angle = 0;

const int stepsPerRev = 200;
Stepper myStepper= Stepper(stepsPerRev, 8, 9, 10, 12);

// Define constants and global variables
int del = 1000;
int enA = 6;
int enB = 11;
char buffer[BUF_SIZE];
int movement;
int pos;
const char* delimiter = ",";

// Control servo speed limits
unsigned long moving_time_limit = 5000; // in ms, defines the maximum milliseconds that motorspeedwrite() can use
unsigned long min_moving_time = 200; // in ms


void setup() {
  // Initialize pins
  myStepper.setSpeed(15);
  servoPan.attach(3);
  servoTilt.attach(5);

  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    int read = Serial.readBytesUntil('\n', buffer, BUF_SIZE - 1);
    buffer[read] = '\0';
    char* first = strtok(buffer, delimiter);
    char* second = strtok(NULL, delimiter);
    char* third = strtok(NULL, delimiter); // potentially and likely will be NULL in most cases

    // No second term, can't run anything below at all
    if (second == NULL) {
      return;
    }

    // Handle negatives, but there shouldn't be any
    if (first[0] == '-') {
      pos = - atoi(first);
    } else {
      pos = atoi(first);
    }
    
    movement = atoi(second);

    // Either of these functions should block the main loop. motorwrite() hardly needs to but motorspeedwrite() contains a loop that slowly increments
    // servo motors. We need a hard limit on how much time a slowed down servo can take
    if (third == NULL) {
      motorwrite(movement, pos, &pan_angle, &tilt_angle);
    } else {
      long duration = atol(third);
      motorspeedwrite(movement, pos, duration, &pan_angle, &tilt_angle);
    }
    
  }
}
