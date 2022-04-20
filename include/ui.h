#include <stdbool.h>

#ifndef UI_LOADED
#define UI_LOADED

//Handles the display, the UI and most of the user-facing logic

typedef struct menu_item
{
    /*
        The name of the menu item.
    */
    char* name;
    /*
        The menu item's ID corresponding to a specific menu.
    */
    int id;
} menu_item;

/*
    @brief Menu containing menu items, the current scroll state and the current selection state.
*/
typedef struct menu
{
    /*
        Determines the selected menu item. 
        If -1, the menu will always return to the main menu. 
        If -2, the menu is for transient use, and is not to be used in the UI loop.
    */
    int selected;
    /*
        Determines the current scroll state of the menu.
    */
    int scroll;
    /*
        The amount of items in the menu.
    */
    int item_count;
    /*
        The array of menu items.
    */
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

/*
    @brief Initialises the button pins and their associated interrupts.
*/
void initialise_buttons();

/*
    @brief Manages the button logic, adding a certain debounce to the button presses. Best called at the start of a UI loop.
*/
void button_logic();

/*
    @brief Starts the LCD and nitialises the UI loop.
    @param version The program version to be displayed at the welcome screen
*/
void initialise_ui(char* version);

/*
    @brief UI loop where menus are sequentially assigned, displayed and discarded.
*/
void ui_loop(menu* first_menu);

/*
    @brief Displays a given menu until a next menu has been selected.
    @param menu The menu to be displayed
    @param next_id The next menu id to be passed to the menu handler and then displayed by the UI loop
*/
void display_menu(menu* menu, int* next_id);

/*
    @brief Frees the current menu in memory and then returns the appropriate menu corresponding to the id.
    @param dmenu The current menu that was displayed
    @param id The menu id of which the corresponding menu is returned
*/
menu* menu_handler(menu* dmenu, int id);

/*
    @brief Creates and returns the main menu.
*/
menu* main_menu();

/*
    @brief Creates and returns the welcome menu.
    @param version The version to be displayed.
*/
menu* welcome_menu(char* version);

/*
    @brief Creates and returns the wordle menu.
*/
menu* wordle_menu();

/*
    @brief Creates and returns the primitives menu.
*/
menu* primitives_menu();

/*
    @brief Creates and returns the manual movement menu.
*/
menu* manual_move_menu();

/*
    @brief Creates and returns an error menu.
    @param reason Reason for the error
*/
menu* error_menu(char* reason);

/*
    @brief Creates and returns a game error menu.
    @param reason The reason for the error
*/
menu* game_error_menu(char* reason);

/*
    @brief Creates and returns a game info menu.
    @param message1 The message on the first line
    @param message2 The message on the second line
*/
menu* game_info_menu(char* message1, char* message2);

/*
    @brief Displays a menu allowing the selection of a letter, then draws that letter.
*/
void letter_select();

/*
    @brief Displays a menu allowing the selection of a word.
    @param attempt_count The attempt count that will be displayed. Use -1 to disable the display of this attempt count.
    @return The word that was selected. Must be freed after use!
*/
char* word_select(int attempt_count);

/*
    @brief Displays a menu allowing the manual movement according to the x and y axes.
*/
void manual_move_xy();

/*
    @brief Displays a menu allowing the manual movement according to the servo angles.
*/
void manual_move_angles();

/*
    @brief Displays a menu allowing the selection of a letter, then draws that letter.
*/
void manual_move_micros();

/*
    @brief Enables servos and displays a transient 'Drawing...' message.
*/
void drawing();
#endif