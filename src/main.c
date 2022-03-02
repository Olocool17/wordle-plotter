#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "dwenguinoBoard.h"
#include "dwenguinoLCD.h"

//hello world from wouter (^3^)

ISR(TIMER1_OVF_vect) { //triggered when the PWM timer reaches the BOTTOM value
  PORTC ^= _BV(PC0);
}

int main(void)
{

  DDRE = 0x00; //Make all PEn pins input pins
  DDRC = _BV(PC0) | _BV(PC1); //Make pins PC0 and PC1 output pins
  PORTE |= _BV(PE4) | _BV(PE5) | _BV(PE6) | _BV(PE7); //make all the button pins activate their pullup-resistors

  SREG |= _BV(SREG_I); //Enable global interrupts

  TIMSK1 |= _BV(OCIE1A);
  TCCR1A &= ~_BV(WGM11); //Enable PWM (phase / frequency correct) clock interrupts
  TCCR1A &= ~_BV(WGM10);
  TCCR1B |= _BV(WGM13);
  TCCR1B &= ~_BV(WGM12);

  TCCR1B &= ~_BV(CS12); //Enable prescaling factor of 1/1 (Required for clock selection)
  TCCR1B &= ~_BV(CS11);
  TCCR1B |= _BV(CS10);

  ICR1 = 0x1900; //6400 : the value of TOP so one PWM 'cycle' is 8ms long
  OCR1A = 0x640;
  
  
  initBoard();
  initLCD();
  clearLCD();

  backlightOn();

  while(1){
    if (!(PINE & _BV(PE4)))
    {
      //Rotate servo 1 left
      OCR1A = 0x640;
    }
    if (!(PINE & _BV(PE5)))
    {
      //Rotate servo 2 left
    }    
    if (!(PINE & _BV(PE6)))
    {
      //Rotate servo 1 right
      OCR1A = 0x320;
    }    
    if (!(PINE & _BV(PE7)))
    {
      //Rotate servo 2 right
    }
  }
  return 0;
}
