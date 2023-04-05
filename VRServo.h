#include <Arduino.h>

// Useful number declarations here that are usually not subject to change
#define SERVO_PAN 0
#define SERVO_TILT 1
#define STEPPER 2
#define BUF_SIZE 15

// Report current position of servos and prints it to the serial monitor for Unity/C# to pick up
void printservopos(const int pan_angle, const int tilt_angle);

/* Takes movetype and position and tells the corresponding motor to move to the position defined. The last two arguments refer to the global value stored
 * by the servos.
 */
void motorwrite(int motortype, int pos, int* pan_angle, int* tilt_angle);

// A function that writes to the servo motors with a set duration.
void motorspeedwrite(int motortype, int pos, long duration, int* pan_angle, int* tilt_angle);

// A rather thin wrapper over motorspeedwrite() but with an input of speed in degrees per second.
void motorcswrite(int motortype, int pos, int speed, int* pan_angle, int* tilt_angle);

bool validmotor(int motortype);

// Note: This function is not likely to be applied on stepper motors.
bool validpos(int pos);


// Some stupid workarounds that I had to do can be found below
