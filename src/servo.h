#ifndef SERVO_LOADED
#define SERVO_LOADED

//Handles the servo initialisation and the accompanying PWM signals.

//Servo 1's duty cycle in microseconds
volatile int servo1_dutymicros;
//Servo 2's duty cycle in microseconds
volatile int servo2_dutymicros;

/*
    @brief Initialises the clock interrupts
*/
void clock_setup();

/* 
    @brief Detaches the clocks, effectively disabling the servos
*/
void servos_disable();

/* 
    @brief Attaches the clocks, sets the clock value and enables the output pins, enabling the servos.
*/
void servos_enable();

#endif