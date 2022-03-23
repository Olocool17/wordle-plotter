#include <stdbool.h>
#include <avr/interrupt.h>

#include "dwenguinoLCD.h"

#include <ui.h>
#include <servo.h>
#include <functions.h>
#include <game.h>

ISR(INT4_vect){
    west_button_pressed = true;
}
ISR(INT5_vect){
    south_button_pressed = true;
}
ISR(INT6_vect){
    east_button_pressed = true;
}
ISR(INT7_vect){
    north_button_pressed = true;
}

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
 
    west_button_pressed = false;
    south_button_pressed = false;
    east_button_pressed = false;
    north_button_pressed = false;

    west_button = false;
    south_button = false;
    east_button = false;
    north_button = false;
}

void button_logic()
{
    west_button = false;
    south_button = false;
    east_button = false;
    north_button = false;
    if (west_button_pressed) west_button = true;
    if (south_button_pressed) south_button = true;
    if (east_button_pressed) east_button = true;
    if (north_button_pressed) north_button = true;
    west_button_pressed = false;
    south_button_pressed = false;
    east_button_pressed = false;
    north_button_pressed = false;
}

void initialise_ui(char* version)
{
    clock_setup();
    initLCD();
    clearLCD();
    backlightOn();
    display_menu(welcome_menu(version));
}

void display_menu(menu* dmenu)
{   
    if(dmenu == NULL){
        dmenu = error_menu("Menu is NULL");
    }

    servos_disable();

    bool back = false;
    while(1)
    { 
        button_logic();
        _delay_ms(100);

        if (west_button && !(PINE & _BV(PE4)))
        {
            back = true;
            break;
        }
        if (east_button && !(PINE & _BV(PE6)))
        {
            break;
        }
        if (south_button && !(PINE & _BV(PE5)))
        {
            if (dmenu->selected + 1 < dmenu->item_count && dmenu->selected != -1)
            {
                dmenu->selected += 1;
            }
            dmenu->scroll = dmenu->selected / 2;
        }
        if (north_button && !(PINE & _BV(PE7)))
        {
            if (dmenu->selected - 1 >= 0)
            {
                dmenu->selected -= 1;
            }
            dmenu->scroll = dmenu->selected / 2;
        }
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
            printCharToLCD('|', dmenu->selected % 2,0);
        }
    }
    _delay_ms(100);
    if (dmenu->selected == -2)
    {
        free(dmenu);
        return;
    }
    else if ((dmenu->selected == -1) | back)
    {
        display_menu(menu_handler(dmenu, 0));
    }
    else
    {
        display_menu(menu_handler(dmenu, dmenu->items[dmenu->selected].id));
    }
}

menu* menu_handler(menu* dmenu, int id)
{
    if (dmenu != NULL) free(dmenu);
    switch (id)
    {
    case 0:
        return main_menu();
        break;
    case 10:
        return wordle_menu();
        break;
    case 11:
        wordle(false);
        return main_menu();
        break;
    case 12:
        wordle(true);
        return main_menu();
        break;
    case 20:
        return primitives_menu();
    case 21:
        //draw grid
        return main_menu();
        break;
     case 22:
        //draw circle
        drawing();
        draw_O(3,3);
        return main_menu();
        break;
    case 23:
        //draw square
        return main_menu();
        break;
    case 24:
        letter_select();
        return main_menu();   
        break;
    case 30:
        manual_move();
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
    mainmenu->item_count = 4;
    mainmenu->items[0].name = "wordle";
    mainmenu->items[0].id = 10;
    mainmenu->items[1].name = "primitives";
    mainmenu->items[1].id = 20;
    mainmenu->items[2].name = "manual move";
    mainmenu->items[2].id = 30;
    mainmenu->items[3].name = "about";
    mainmenu->items[3].id = 40;
    return mainmenu;
}

menu* wordle_menu()
{
    menu* wordlemenu = malloc(sizeof(menu));
    wordlemenu->selected = 0;
    wordlemenu->scroll = 0;
    wordlemenu->item_count = 2;
    wordlemenu->items[0].name = "manual word";
    wordlemenu->items[0].id = 11;
    wordlemenu->items[1].name = "random word";
    wordlemenu->items[1].id = 12;
    return wordlemenu;
}

menu* primitives_menu()
{
    menu* primitivesmenu = malloc(sizeof(menu));
    primitivesmenu->selected = 0;
    primitivesmenu->scroll = 0;
    primitivesmenu->item_count = 4;
    primitivesmenu->items[0].name = "grid";
    primitivesmenu->items[0].id = 21;
    primitivesmenu->items[1].name = "circle";
    primitivesmenu->items[1].id = 22;
    primitivesmenu->items[2].name = "square";
    primitivesmenu->items[2].id = 23;
    primitivesmenu->items[3].name = "letter";
    primitivesmenu->items[3].id = 24;
    return primitivesmenu;
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

menu* game_info_menu(char* message)
{
    menu* info = malloc(sizeof(menu));
    info->selected = -2;
    info-> scroll = 0;
    info->item_count = 1;
    info->items[0].name = message;
    info->items[0].id = 0;
    return info;
}

void letter_select()
{
    char letter = 'A';
    bool back = false;
    while(1)
    {
        button_logic();
        _delay_ms(100);
        if (west_button && !(PINE & _BV(PE4)))
        {
            back = true;
            break;
        }
        if (east_button && !(PINE & _BV(PE6)))
        {
            break;
        }
        if (south_button && !(PINE & _BV(PE5)))
        {
            if ((int) letter >  65)
            {
                letter -= 1;
            }
            else
            {
                letter = 'Z';
            }
        }
        if (north_button && !(PINE & _BV(PE7)))
        {
            if ((int)letter < 90)
            {
                letter += 1;
            }
            else
            {
                letter = 'A';
            }
        }
        clearLCD();
        printCharToLCD(letter,0,8);
    }
    if (!back) //draw letter;
    return;
}

char* word_select()
{
    char word[5] = "AAAAA";
    int selection = 0;
    bool exit = false;
    while(1)
    {
        button_logic();
        _delay_ms(100);
        if (west_button && !(PINE & _BV(PE4)))
        {
            if (selection == 0)
            {
                exit = true;
                break;
            }
            else
            {
                selection -= 1;
            }
        }
        if (east_button && !(PINE & _BV(PE6)))
        {
            if (selection == 4)
            {
                break;
            }
            else
            {
                selection += 1;
            }
        }
        if (south_button && !(PINE & _BV(PE5)))
        {
            if ((int) word[selection] >  65)
            {
                word[selection] -= 1;
            }
            else
            {
                word[selection] = 'Z';
            }
        }
        if (north_button && !(PINE & _BV(PE7)))
        {
            if ((int)word[selection] < 90)
            {
                word[selection] += 1;
            }
            else
            {
                word[selection] = 'A';
            }
        }
        clearLCD();
        printStringToLCD("exit",0,0);
        printStringToLCD(word,0,6);
        printStringToLCD("   go",0,11);
        printCharToLCD('^',1,6 + selection);
    }
    if (exit) return "00000";
    return word;
}

void manual_move()
{ 
    clock_setup();
    servos_enable();
    while(1){
        _delay_ms(100);
        clearLCD();
        printStringToLCD("MANUAL CONTROL",0,0);
        if (!(PINE & _BV(PE4)) && !(PINE & _BV(PE5)) &&  !(PINE & _BV(PE6)) && !(PINE & _BV(PE7)))
        {
            servos_disable();
            return;
        }
        if (!(PINE & _BV(PE4)))
        {
            //Rotate servo 1 left
            printStringToLCD("Servo 1, left ", 0, 0);
            printIntToLCD(servo1_dutymicros, 1, 0);
            servo1_dutymicros -= 10;
        }
        if (!(PINE & _BV(PE5)))
        {
        //Rotate servo 2 left
            printStringToLCD("Servo 2, left ", 0, 0);
            printIntToLCD(servo2_dutymicros , 1, 0);
            servo2_dutymicros -= 10;
        }    
        if (!(PINE & _BV(PE6)))
        {
            //Rotate servo 1 right
            printStringToLCD("Servo 1, right", 0, 0);
            printIntToLCD(servo1_dutymicros, 1, 0);
            servo1_dutymicros += 10;
        }    
        if (!(PINE & _BV(PE7)))
        {
            //Rotate servo 2 right
            printStringToLCD("Servo 2, right", 0, 0);
            printIntToLCD(servo2_dutymicros, 1, 0);
            servo2_dutymicros += 10;
        }
    }
}
void drawing()
{
        servos_enable();
        clearLCD();
        printStringToLCD("drawing...",0,4);
}