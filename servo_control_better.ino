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
unsigned long moving_time_limit = 18000; // in ms, defines the maximum milliseconds that motorspeedwrite() can use
unsigned long min_moving_time = 200; // in ms

/* DESCRIPTION
 * The way how this program works is that it serves to interact with a C# wrapper function to control a robotic arm. Alternatively, this can be done via
 * the serial monitor by entering numbers separated by commas. 
 * For two inputs: <angle>,<motorindex> with no angle brackets (example such as 50,0).
 * For 3 inputs: <angle>,<motorindex>,<duration> with no angle brackets
 */


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
