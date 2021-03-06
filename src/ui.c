#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "dwenguinoLCD.h"

#include <ui.h>
#include <servo.h>
#include <functions.h>
#include <game.h>

#define REFRESH_MS (75)

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
    initLCD();
    clearLCD();
    backlightOn();
    ui_loop(welcome_menu(version));
}

void ui_loop(menu* firstmenu)
{
    menu* current_menu = firstmenu;
    while(1)
    {
        int next_id = -1;
        display_menu(current_menu, &next_id);
        current_menu = menu_handler(current_menu, next_id);
    }
}

void display_menu(menu* dmenu, int* next_id)
{   
    if(dmenu == NULL){
        dmenu = error_menu("Menu is NULL");
    }
    servos_disable();
    bool back = false;
    while(1)
    { 
        button_logic();
        _delay_ms(REFRESH_MS);

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
        if (dmenu->selected < 0) 
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
    _delay_ms(REFRESH_MS);
    if (dmenu->selected == -2)
    {
        free(dmenu);
        return;
    }
    else if ((dmenu->selected == -1) | back)
    {
        if (next_id != NULL) *next_id = 0;
    }
    else
    {
        if (next_id != NULL) *next_id = dmenu->items[dmenu->selected].id;
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
        draw_grid();
        return main_menu();
        break;
     case 22:
        draw_circle();
        return main_menu();
        break;
    case 23:
        draw_square();
        return main_menu();
        break;
    case 24:
        letter_select();
        return main_menu();   
        break;
    case 30:
        return manual_move_menu();
        break;
    case 31:
        manual_move_xy();
        return main_menu();
        break;
    case 32:
        manual_move_angles();
        return main_menu();
        break;
    case 33:
        manual_move_micros();
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

menu* manual_move_menu()
{
    menu* manualmenu = malloc(sizeof(menu));
    manualmenu->selected = 0;
    manualmenu->scroll = 0;
    manualmenu->item_count = 3;
    manualmenu->items[0].name = "xy";
    manualmenu->items[0].id = 31;
    manualmenu->items[1].name = "angles";
    manualmenu->items[1].id = 32;
    manualmenu->items[2].name = "pwm micros";
    manualmenu->items[2].id = 33;
    return manualmenu;
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

menu* game_error_menu(char* reason) 
{
    menu* error = malloc(sizeof(menu));
    error->selected = -2;
    error->scroll = 0;
    error->item_count = 2;
    error->items[0].name = "ERROR";
    error->items[0].id = 0;
    error->items[1].name = reason;
    error->items[1].id = 0;
    return error;
}

menu* game_info_menu(char* message1, char* message2)
{
    menu* info = malloc(sizeof(menu));
    info->selected = -2;
    info-> scroll = 0;
    info->item_count = 2;
    info->items[0].name = message1;
    info->items[0].id = 0;
    info->items[1].name = message2;
    info->items[1].id = 0;
    return info;
}

void letter_select()
{
    char letter = 'A';
    bool back = false;
    while(1)
    {
        button_logic();
        _delay_ms(REFRESH_MS);
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
    if (!back) draw_letter(letter, 1600, 100);
    return;
}

char* word_select(int attempt_count)
{
    servos_disable();
    //displaying 'AAAAA' to user at the start of word selection
    char* word = malloc(sizeof(char)*6);
    for (size_t i = 0; i < 5; i++)
    {
        word[i] = 'A';
    }
    word[5] = '\0';
    int selection = 0;
    bool exit = false;
    //handling the word selection
    while(1)
    {
        button_logic();
        _delay_ms(REFRESH_MS);
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
        //showing the user what attempt they are currently on
        if (attempt_count != -1)
        {
            printCharToLCD('(', 1, 13);
            printIntToLCD( attempt_count + 1, 1, 14);
            printCharToLCD(')', 1, 15);
        }
    }
    if (exit)
    {
        free(word);
        return "00000";
    } 
    return word;
}

void manual_move_micros()
{ 
    servos_enable();
    while(1){
        _delay_ms(REFRESH_MS);
        clearLCD();
        if (!(PINE & _BV(PE4)) && !(PINE & _BV(PE5)) &&  !(PINE & _BV(PE6)) && !(PINE & _BV(PE7)))
        {
            servos_disable();
            return;
        }
        else if (!(PINE & _BV(PE4)))
        {
            //Rotate servo 1 left
            printStringToLCD("servo 1, left ", 0, 0);
            printIntToLCD(servo1_dutymicros, 1, 0);
            servo1_dutymicros -= 10;
        }
        else if (!(PINE & _BV(PE5)))
        {
        //Rotate servo 2 left
            printStringToLCD("servo 2, left ", 0, 0);
            printIntToLCD(servo2_dutymicros , 1, 0);
            servo2_dutymicros -= 10;
        }    
        else if (!(PINE & _BV(PE6)))
        {
            //Rotate servo 1 right
            printStringToLCD("servo 1, right", 0, 0);
            printIntToLCD(servo1_dutymicros, 1, 0);
            servo1_dutymicros += 10;
        }    
        else if (!(PINE & _BV(PE7)))
        {
            //Rotate servo 2 right
            printStringToLCD("servo 2, right", 0, 0);
            printIntToLCD(servo2_dutymicros, 1, 0);
            servo2_dutymicros += 10;
        }
        else
        {
            printStringToLCD("manual control:",0,0);
            printStringToLCD("pwm micros", 1, 0);
        }
    }
}

void manual_move_angles()
{ 
    servos_enable();
    int servo1_angle = 90;
    int servo2_angle = 90;
    clearLCD();
    printStringToLCD("manual control:",0,0);
    printStringToLCD("angles", 1, 0);
    while(1){
        _delay_ms(REFRESH_MS);
        if (!(PINE & _BV(PE4)) && !(PINE & _BV(PE5)) &&  !(PINE & _BV(PE6)) && !(PINE & _BV(PE7)))
        {
            servos_disable();
            return;
        }
        else if (!(PINE & _BV(PE4)))
        {
            //Rotate servo 1 left
            servo1_angle -= 1;
            clearLCD();
            printStringToLCD("servo 1, left ", 0, 0);
            printIntToLCD(servo1_angle, 1, 0);
            printStringToLCD("pwm:", 1, 4);
            printIntToLCD(servo1_dutymicros, 1, 8);
        }
        else if (!(PINE & _BV(PE5)))
        {
            //Rotate servo 2 left
            servo2_angle -= 1;
            clearLCD();
            printStringToLCD("servo 2, left ", 0, 0);
            printIntToLCD(servo2_angle , 1, 0);
            printStringToLCD("pwm:", 1, 4);
            printIntToLCD(servo2_dutymicros, 1, 8);
        }    
        else if (!(PINE & _BV(PE6)))
        {
            //Rotate servo 1 right
            servo1_angle += 1;
            clearLCD();
            printStringToLCD("servo 1, right", 0, 0);
            printIntToLCD(servo1_angle, 1, 0);
            printStringToLCD("pwm:", 1, 4);
            printIntToLCD(servo1_dutymicros, 1, 8);
        }    
        else if (!(PINE & _BV(PE7)))
        {
            //Rotate servo 2 right
            servo2_angle += 1;
            clearLCD();
            printStringToLCD("servo 2, right", 0, 0);
            printIntToLCD(servo2_angle, 1, 0);
            printStringToLCD("pwm:", 1, 4);
            printIntToLCD(servo2_dutymicros, 1, 8);
        }
        move(radians_to_micros((float)servo1_angle * M_PI / 180), radians_to_micros((float)servo2_angle * M_PI / 180));
    }
}

void manual_move_xy()
{ 
    servos_enable();
    long int x = 500;
    long int y = 500;
    while(1){
        _delay_ms(REFRESH_MS);
        clearLCD();
        move_xy(x, y);
        if (!(PINE & _BV(PE4)) && !(PINE & _BV(PE5)) &&  !(PINE & _BV(PE6)) && !(PINE & _BV(PE7)))
        {
            servos_disable();
            return;
        }
        else if (!(PINE & _BV(PE4)))
        {
            //Rotate servo 1 left
            printStringToLCD("x, left ", 1, 0);
            printIntToLCD(x, 1, 10);
            x -= 10;
        }
        else if (!(PINE & _BV(PE5)))
        {
        //Rotate servo 2 left
            printStringToLCD("y, down ", 1, 0);
            printIntToLCD(y , 1, 10);
            y-= 10;
        }    
        else if (!(PINE & _BV(PE6)))
        {
            //Rotate servo 1 right
            printStringToLCD("x, right", 1, 0);
            printIntToLCD(x, 1, 10);
            x += 10;
        }    
        else if (!(PINE & _BV(PE7)))
        {
            //Rotate servo 2 right
            printStringToLCD("y, up", 1, 0);
            printIntToLCD(y, 1, 10);
            y += 10;
        }
        else
        {
            printStringToLCD("manual control:",0,0);
            printStringToLCD("xy", 1, 0);
        }
    }
}

void drawing()
{
        servos_enable();
        clearLCD();
        printStringToLCD("drawing...",0,4);
}