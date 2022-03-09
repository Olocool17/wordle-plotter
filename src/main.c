#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdbool.h>
#include "dwenguinoBoard.h"
#include "dwenguinoLCD.h"

//hello world from wouter (^3^)s

volatile bool pwm = 0;

ISR(TIMER1_COMPA_vect) {
  pwm = true;
}

void servo_pwm_generator(bool servos_enabled, int dutymicros1, int dutymicros2)
  {
    if (pwm && servos_enabled){
      pwm = false;
      if (dutymicros1 > dutymicros2){
        int dutydiff = dutymicros1 - dutymicros2;
        PORTC |= _BV(PC0);
        PORTC |= _BV(PC1);
        _delay_us(dutymicros2);
        PORTC &= ~_BV(PC1);
        _delay_us(dutydiff);
        PORTC &= ~_BV(PC0);
      }
      else if (dutymicros2 > dutymicros1) {
        int dutydiff = dutymicros2 - dutymicros1;
        PORTC |= _BV(PC0);
        PORTC |= _BV(PC1);
        _delay_us(dutymicros1);
        PORTC &= ~_BV(PC0);
        _delay_us(dutydiff);
        PORTC &= ~_BV(PC1);
      }
      else {
        PORTC |= _BV(PC0);
        PORTC |= _BV(PC1);
        _delay_us(dutymicros1);
        PORTC &= ~_BV(PC0);
        PORTC &= ~_BV(PC1);
      }
    }
  }

int main(void)
{
  DDRE = 0x00; //Make all PEn pins input pins
  DDRC = _BV(PC0) | _BV(PC1); //Make pins PC0 and PC1 output pins
  PORTE |= _BV(PE4) | _BV(PE5) | _BV(PE6) | _BV(PE7); //make all the button pins activate their pullup-resistors

  SREG |= _BV(SREG_I); //Enable global interrupts

  TIMSK1 |= _BV(OCIE1A); //Enable clock interrupts with CTC
  TCCR1A &= ~_BV(WGM11);
  TCCR1A &= ~_BV(WGM10);
  TCCR1B &= ~_BV(WGM13);
  TCCR1B |= _BV(WGM12);

  TCCR1B &= ~_BV(CS12); //Enable prescaling factor of 1/8 (Required for clock selection)
  TCCR1B |= _BV(CS11);
  TCCR1B &= ~_BV(CS10);

  OCR1A = 0x4E20; //20 000 : the amount of 8 clock cycles that corresponds to 10 ms at 16 MHz

  long int servo1_dutymicros = 1500;
  long int servo2_dutymicros = 1500;

  bool servos_enabled = true;

  initBoard();
  initLCD();
  clearLCD();

  backlightOn();

  while(1){
    servo_pwm_generator(servos_enabled, servo1_dutymicros, servo2_dutymicros);
    if (!(PINE & _BV(PE4)))
    {
      //Rotate servo 1 left
      printStringToLCD("Servo 1, left ", 0, 0);
      printIntToLCD(servo1_dutymicros - servo2_dutymicros, 1, 0);
      servo1_dutymicros -= 1;
    }
    if (!(PINE & _BV(PE5)))
    {
      //Rotate servo 2 left
      printStringToLCD("Servo 2, left ", 0, 0);
      printIntToLCD(servo1_dutymicros - servo2_dutymicros, 1, 0);
      servo2_dutymicros -= 1;
    }    
    if (!(PINE & _BV(PE6)))
    {
      //Rotate servo 1 right
      printStringToLCD("Servo 1, right", 0, 0);
      printIntToLCD(servo1_dutymicros - servo2_dutymicros, 1, 0);
      servo1_dutymicros += 1;
    }    
    if (!(PINE & _BV(PE7)))
    {
      //Rotate servo 2 right
      printStringToLCD("Servo 2, right", 0, 0);
      printIntToLCD(servo1_dutymicros - servo2_dutymicros, 1, 0);
      servo2_dutymicros += 1;
    }
  }
  return 0;
}
