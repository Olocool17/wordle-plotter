#ifndef SERVO_LOADED
#define SERVO_LOADED

volatile int servo1_dutymicros;
volatile int servo2_dutymicros;

void clock_setup();

void servos_disable();

void servos_enable();

#endif