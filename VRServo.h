#include <Arduino.h>

// Useful number declarations here
#define SERVO_PAN 0
#define SERVO_TILT 1
#define STEPPER 2

/* Takes movetype and position and tells the corresponding motor to move to the position defined. The last two arguments refer to the global value stored
 * by the servos.
 */
void motorwrite(int motortype, int pos, int* pan_angle, int* tilt_angle);

void motorspeedwrite(int motortype, int pos, int duration, int* pan_angle, int* tilt_angle);