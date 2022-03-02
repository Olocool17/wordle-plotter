#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "dwenguinoBoard.h"
#include "dwenguinoLCD.h"

volatile unsigned char spin_switch = 0;
volatile unsigned int spin_index = 0;

//hello world from wouter (^3^)

unsigned int spin_mode = 0;

const char left_slot[8] = {'a','e','b','f','d','c','h','g'};
const char middle_slot[8] = {'f','h','g','b','a','c','e','d'};
const char right_slot[8] = {'b','c','h','f','a','g','d','e'};

unsigned int spent_money = 0;
unsigned int earned_money = 0;

ISR(INT4_vect){
  spin_switch = 1;
}

ISR(TIMER1_COMPA_vect) {
  if (spin_mode)
  {
    spin_index++;
  }
}

int main(void)
{

  DDRE = 0x00; //Make all PEn pins input pins
  PORTE |= _BV(PE4) | _BV(PE5) | _BV(PE6) | _BV(PE7); //make all the button pins activate their pullup-resistors

  SREG |= _BV(SREG_I); //Enable global interrupts
  EIMSK |= _BV(INT4); // Enable interrupts for pin 4

  EICRB |= _BV(ISC41); // Generate interrupt on falling edge
  EICRB &= ~_BV(ISC40);

  TIMSK1 |= _BV(OCIE1A); //Enable clock interrupts with CTC
  TCCR1A &= ~_BV(WGM11);
  TCCR1A &= ~_BV(WGM10);
  TCCR1B &= ~_BV(WGM13);
  TCCR1B |= _BV(WGM12);

  TCCR1B |= _BV(CS12); //Enable prescaling factor of 1/1024
  TCCR1B &= ~_BV(CS11);
  TCCR1B |= _BV(CS10);

  OCR1A = 0x1453; //5203 : the amount of 1024 clock cycles during 0.333 seconds at a clock speed of 16MHz
  
  initBoard();
  initLCD();
  clearLCD();

  backlightOn();

  while(1){
    if (!(PINE & _BV(PE4)))
    {
      //Rotate servo 1 left
    }
    if (!(PINE & _BV(PE5)))
    {
      //Rotate servo 2 left
    }    
    if (!(PINE & _BV(PE6)))
    {
      //Rotate servo 1 right
    }    
    if (!(PINE & _BV(PE7)))
    {
      //Rotate servo 2 right
    }        
  }
  return 0;
}
