#ifndef UI_LOADED
#define UI_LOADED

typedef struct menu_item{
    char* name;
    int id;
} menu_item;

typedef struct menu{
    int selected; 
    int scroll;
    int item_count;
    menu_item items[];
} menu;

volatile bool west_button_pressed;
volatile bool south_button_pressed;
volatile bool east_button_pressed;
volatile bool north_button_pressed;

bool west_button;
bool south_button;
bool east_button;
bool north_button ;

//Initialises the buttons
void initialise_buttons();

//Calls the button logic: this should be put before the delay in your UI while loop
void button_logic();

//Initialises the UI, LCD and buttons/interrupts
void initialise_ui(char* version);

//Enters the display loop of a menu, also handles buttons
void display_menu(menu* menu);

//Returns the menu that corresponds to the ID and frees the previous menu from memory
menu* menu_handler(menu* dmenu, int id);
//Returns the main menu
menu* main_menu();

//Returns the welcome menu with the appropriate version
menu* welcome_menu(char* version);

//Returns the wordle menu
menu* wordle_menu();

//Returns the primitives menu
menu* primitives_menu();

//Returns the error menu with the appropriate reason
menu* error_menu(char* reason);

void letter_select();

void word_select();

void manual_move();
#endif