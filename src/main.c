#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/delay.h>

#include "dwenguinoBoard.h"
#include "dwenguinoLCD.h"

#include <ui.h>
#include <servo.h>

#define VERSION "v0.0"

//hello world from wouter (^3^)s

int main(void)

{
  initialise_buttons();
  initLCD();
  clearLCD();
  backlightOn();
  initialise_ui(VERSION);
  //manual_move();
  return 0;
}
