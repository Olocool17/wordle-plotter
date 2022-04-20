#include <ui.h>
#include <servo.h>

#define VERSION "v0.0"

//hello world from wouter (^3^)s

int main(void)
{
  initialise_buttons();
  clock_setup();
  initialise_ui(VERSION);
  return 0;
}