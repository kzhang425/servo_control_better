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
char buffer[10];
int movement;
int pos;
const char* delimiter = ",";
unsigned long move_start_time;
unsigned long moving_time_limit = 5000; // in ms, defines the maximum milliseconds that motorspeedwrite() can use


void setup() {
  // Initialize pins
  myStepper.setSpeed(15);
  servoPan.attach(3);
  servoTilt.attach(5);

  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);

  Serial.begin(9600);
  move_start_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    int read = Serial.readBytesUntil('\n', buffer, 9);
    buffer[read] = '\0';
    char* first = strtok(buffer, delimiter);
    char* second = strtok(NULL, delimiter);
    char* third = strtok(NULL, delimiter); // potentially and likely will be NULL in most cases
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
      int duration = atoi(third);
      motorspeedwrite(movement, pos, duration, &pan_angle, &tilt_angle);
    }
    
  }
}
