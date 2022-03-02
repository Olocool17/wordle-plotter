#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "dwenguinoBoard.h"
#include "dwenguinoLCD.h"

volatile unsigned char spin_switch = 0;
volatile unsigned int spin_index = 0;

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


  char current_slots[3] = {left_slot[0], middle_slot[0], right_slot[0]};
  char next_slots[3] = {left_slot[1], middle_slot[1], right_slot[1]};

  while(1){

    if (spin_switch)
    {
      _delay_ms(30);
      spin_switch = 0;

      if (spin_mode == 0)
      {
        spent_money++;
      }
      spin_mode++;
    }

    switch (spin_mode)
    {
      case 1:
        current_slots[0] = left_slot[spin_index % 8];
        current_slots[1] = middle_slot[spin_index % 8];
        current_slots[2] = right_slot[spin_index % 8];
        next_slots[0] = left_slot[(spin_index + 1)% 8];
        next_slots[1] = middle_slot[(spin_index + 1) % 8];
        next_slots[2] = right_slot[(spin_index + 1) % 8];
        break;
      case 2:
        current_slots[1] = middle_slot[spin_index % 8];
        current_slots[2] = right_slot[spin_index % 8];
        next_slots[1] = middle_slot[(spin_index + 1) % 8];
        next_slots[2] = right_slot[(spin_index + 1) % 8];
        break;
      case 3:
        current_slots[2] = right_slot[spin_index % 8];
        next_slots[2] = left_slot[(spin_index + 1)% 8];
        break;
      case 4:
        if (current_slots[0] == current_slots[1] && current_slots[1] == current_slots[2])
        {
          earned_money += 7;
        }
        spin_mode = 0;
        spin_index = 0;
        break;
    }

    printStringToLCD(current_slots, 0, 0);
    printIntToLCD(spent_money, 0, 13);
    printStringToLCD(next_slots, 1, 0);
    printIntToLCD(earned_money, 1, 13);
    _delay_ms(333);
    
  }
  return 0;
}
