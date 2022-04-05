#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include <servo.h>

volatile bool servo1_pwm = false; // true if PWM signal is high
volatile bool servo2_pwm = false;
volatile int servo1_dutymicros = 1500;
volatile int servo2_dutymicros = 1500;

ISR(TIMER1_COMPA_vect) 
{
  if (servo1_pwm)
  {
    PORTC &= ~_BV(PC0); //set PWM signal to low
    servo1_pwm = false;
    OCR1A = 2 * (10000 - servo1_dutymicros); // duration of PWM low cycle
  }
  else 
  {
    servo1_pwm = true;
    PORTC |= _BV(PC0); // set PWM signal to high
    OCR1A = 2 * servo1_dutymicros; // duration of PWM high cycle
  }
}

ISR(TIMER3_COMPA_vect) 
{
  if (servo2_pwm)
  {
    PORTC &= ~_BV(PC1); //set PWM signal to low
    servo2_pwm = false;
    OCR3A = 2 * (10000 - servo2_dutymicros); // duration of PWM low cycle
  }
  else 
  {
    servo2_pwm = true;
    PORTC |= _BV(PC1); // set PWM signal to high
    OCR3A = 2 * servo2_dutymicros; // duration of PWM high cycle
  }
}

void clock_setup()
{
  SREG |= _BV(SREG_I); //Enable global interrupts

  TIMSK1 |= _BV(OCIE1A); //Enable clock interrupts with CTC for timer 1
  TCCR1A &= ~_BV(WGM10);
  TCCR1A &= ~_BV(WGM11);
  TCCR1B |= _BV(WGM12);
  TCCR1B &= ~_BV(WGM13);

  TIMSK3 |= _BV(OCIE3A); //Enable clock interrupts with CTC for timer 3
  TCCR3A &= ~_BV(WGM30);
  TCCR3A &= ~_BV(WGM31);
  TCCR3B |= _BV(WGM32);
  TCCR3B &= ~_BV(WGM33);
}

void servos_disable()
{
  TCCR1B &= ~_BV(CS12); //Detaches the clock, effectively disabling servo 1
  TCCR1B &= ~_BV(CS11);
  TCCR1B &= ~_BV(CS10);
  TCCR3B &= ~_BV(CS32); //Detaches the clock, effectively disabling servo 2
  TCCR3B &= ~_BV(CS31);
  TCCR3B &= ~_BV(CS30);

  PORTC &= _BV(PC0); //set PWM signal to low
  PORTC &= _BV(PC1);
}

void servos_enable()
{
  DDRC |= _BV(PC0) | _BV(PC1); //Make pins PC0 and PC1 output pins
  TCCR1B &= ~_BV(CS12); //Selects clock with prescaling factor of 1/8 , enabling servo 1
  TCCR1B |= _BV(CS11);
  TCCR1B &= ~_BV(CS10);
  TCCR3B &= ~_BV(CS32); //Selects clock with prescaling factor of 1/8 , enabling servo 2
  TCCR3B |= _BV(CS31);
  TCCR3B &= ~_BV(CS30);

  servo1_pwm = false;
  servo2_pwm = false;

  OCR1A = 2*10000; // Clock interrupts every 10 ms
  OCR3A = 2*10000; 
}