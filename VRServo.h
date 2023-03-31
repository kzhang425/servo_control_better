#include <Arduino.h>

// Useful number declarations here
#define SERVO_PAN 0
#define SERVO_TILT 1
#define STEPPER 2

// Takes movetype and position and tells the corresponding motor to move to the position defined.
void motorwrite(int motortype, int pos);