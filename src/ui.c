#include <stdbool.h>

#include "dwenguinoLCD.h"

#include <ui.h>

void initialise_buttons()
{
    //Set button pins as inputs
    DDRC &= ~_BV(PC7);
    DDRE &= ~_BV(PE7);
    DDRE &= ~_BV(PE6);
    DDRE &= ~_BV(PE5);
    DDRE &= ~_BV(PE4);
    //Enable pull-up resistors
    PORTC |= _BV(PC7);
    PORTE |= _BV(PE7);
    PORTE |= _BV(PE6);
    PORTE |= _BV(PE5);
    PORTE |= _BV(PE4);
    /*
    //Enable global interrupts
    SREG |= _BV(SREG_I); 
    //Enable external interrupts
    EIMSK |= _BV(INT7); 
    EIMSK |= _BV(INT6);
    EIMSK |= _BV(INT5);
    EIMSK |= _BV(INT4);
    //Generate interrupt on falling edge
    EICRB |= _BV(ISC71); 
    EICRB &= ~_BV(ISC70);
    EICRB |= _BV(ISC61); 
    EICRB &= ~_BV(ISC60);
    EICRB |= _BV(ISC51); 
    EICRB &= ~_BV(ISC50);
    EICRB |= _BV(ISC41); 
    EICRB &= ~_BV(ISC40);
    */
}

void initialise_ui(char* version)
{
    initLCD();
    clearLCD();
    backlightOn();
    display_menu(welcome_menu(version));
}

void display_menu(menu* dmenu)
{   
    menu* next_menu_item;
    bool middle_button_pressed = false;  
    bool west_button_pressed = false;
    bool south_button_pressed = false;
    bool east_button_pressed = false;
    bool north_button_pressed = false;
    while(1)
    { 
        if (middle_button_pressed && !(PORTC & _BV(PC7)))
        {
            if (dmenu->selected == -1)
            {
                menu* next_menu_item = menu_handler(0);
            }
            else
            {
                menu* next_menu_item = menu_handler(dmenu->items[dmenu->selected].id);
            }
            break;
        }
        if (south_button_pressed && !(PORTE & _BV(PE6)))
        {
            if (dmenu->selected + 1 < dmenu->item_count && dmenu->selected != -1)
            {
                dmenu->selected += 1;
            }
            dmenu->scroll = dmenu->selected / 2;
        }
        if (north_button_pressed && !(PORTE & _BV(PE4)))
        {
            if (dmenu->selected - 1 >= 0)
            {
                dmenu->selected -= 1;
            }
            dmenu->scroll = dmenu->selected / 2;
        }
        middle_button_pressed = false;
        west_button_pressed = false;
        south_button_pressed = false;
        east_button_pressed = false;
        north_button_pressed = false;
        if (!(PORTC & _BV(PC7))) middle_button_pressed = true;
        if (!(PORTE & _BV(PE7))) west_button_pressed = true;
        if (!(PORTE & _BV(PE6))) south_button_pressed = true;
        if (!(PORTE & _BV(PE5))) east_button_pressed = true;
        if (!(PORTE & _BV(PE4))) north_button_pressed = true;

        clearLCD();
        if (dmenu->selected == -1) 
        {
            printStringToLCD(dmenu->items[0 + dmenu->scroll*2].name, 0, 0);
            if (1 + dmenu->scroll*2 < dmenu->item_count)
            {
                printStringToLCD(dmenu->items[1 + dmenu->scroll*2].name, 1, 0);
            }
        }
        else //Display with cursor
        {
            printStringToLCD(dmenu->items[0 + dmenu->scroll*2].name, 0, 1);
            if (1 + dmenu->scroll*2 < dmenu->item_count)
            {
                printStringToLCD(dmenu->items[1 + dmenu->scroll*2].name, 1, 1);
            }
            printCharToLCD('+', dmenu->selected % 2,0);
        }
        _delay_ms(100);
    }
    display_menu(next_menu_item);
}

menu* menu_handler(int id)
{
    switch (id)
    {
    case 0:
        return main_menu();
        break;
    
    default:
        return error_menu("Invalid menu ID");
        break;
    }

}

menu* main_menu()
{
    menu* mainmenu = malloc(sizeof(menu));
    mainmenu->selected = 0;
    mainmenu->scroll = 0;
    mainmenu->item_count = 3;
    mainmenu->items[0].name = "wordle";
    mainmenu->items[0].id = 1;
    mainmenu->items[1].name = "primitives";
    mainmenu->items[1].id = 2;
    mainmenu->items[2].name = "about";
    mainmenu->items[2].id = 3;
    return mainmenu;
}

menu* welcome_menu(char* version)
{
    menu* welcome = malloc(sizeof(menu));
    welcome->selected = -1;
    welcome->scroll = 0;
    welcome->item_count = 2;
    welcome->items[0].name = "wordle-plotter";
    welcome->items[0].id = 0;
    welcome->items[1].name = version;
    welcome->items[1].id = 0;
    return welcome;
}

menu* error_menu(char* reason) 
{
    menu* error = malloc(sizeof(menu));
    error->selected = -1;
    error->scroll = 0;
    error->item_count = 2;
    error->items[0].name = "ERROR";
    error->items[0].id = 0;
    error->items[1].name = reason;
    error->items[1].id = 0;
    return error;
}