#ifndef FUNCTIONS_LOADED
#define FUNCTIONS_LOADED

//Handles the drawing logic, inverse kinematics and shape functions.


/*
    @brief Converts an angle in radians to the PWM duty cycle in microseconds.
    @param rad The angle in radians
    @return The corresponding angle in PWM duty cycle in microseconds
*/
int radians_to_micro(float rad);

/*
    @brief Moves the arms to the corresponding duty cycles. Must be a value between 700 and 2300.
    @param theta_1 The target microsecond duty cycle for servo 1.
    @param theta_2 The target microsecond duty cycle for servo 2.
*/
void move(int theta_1, int theta_2);

/*
    @brief Moves the arms to the corresponding x and y coordinates.
    @param x_coor The target x coordinate
    @param y_coor The target y coordinate
*/
void move_xy(int x_coor, int y_coor);

/*
    @brief Moves the arms to the corresponding x and y coordinates with the pen lifted.
    @param x_coor The target x coordinate
    @param y_coor The target y coordinate
*/
void move_xy_with_lift(int x_coor, int y_coor);

/*
    @brief Checks if the given coordinates are within the bounds of the board.
    @param x_coor The x coordinate to be checked
    @param y_coor The y coordinate to be checked
    @return Whether or not the coordinates are within the bounds of the board
*/
bool within_bounds(int x_coor, int y_coor);

/*
    @brief Moves the pen in a lineair bezier curve (= a straight line).
    @param start_x The x coordinate of the start point
    @param start_y The y coordinate of the start point
    @param end_x The x coordinate of the end point
    @param end_y The y coordinate of the end point
*/
void lin_bez(int start_x, int start_y, int end_x, int end_y);
/*
    @brief Moves the pen in a cubic bezier curve.
    @param start_x The x coordinate of the start point
    @param start_y The y coordinate of the start point
    @param cp1_x The x coordinate of the first control point
    @param cp1_y The y coordinate of the first control point
    @param cp2_x The x coordinate of the second control point
    @param cp2_y The y coordinate of the second control point
    @param end_x The x coordinate of the end point
    @param end_y The y coordinate of the end point
*/
void cub_bez(int start_x, int start_y, int cp1_x, int cp1_y, int cp2_x, int cp2_y, int end_x, int end_y);


//Primitives

/*
    @brief Draws a square grid on the board.
*/
void draw_grid();

/*
    @brief Draws the given letter at the given coordinates (lower left corner of the letter's 'box')
    @param letter The letter to be drawn
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_letter(char letter, int x, int y);

/*
    @brief Draws the given letter inside of a tile in the letter 'grid'. (0,0) is the top left tile in this grid.
    @param letter The letter to be drawn
    @param tile_x The x-coordinate for the tile
    @param tile_y The y-coordinate for the tile
*/
void draw_letter_on_grid(char letter, int tile_x, int tile_y);

//Functions for individual letters

/*
    @brief Draws the letter A at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_A( int x, int y);

/*
    @brief Draws the letter B at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_B( int x, int y);

/*
    @brief Draws the letter C at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_C( int x, int y);

/*
    @brief Draws the letter D at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_D( int x, int y);

/*
    @brief Draws the letter E at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_E( int x, int y);

/*
    @brief Draws the letter F at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_F( int x, int y);

/*
    @brief Draws the letter G at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_G( int x, int y);

/*
    @brief Draws the letter H at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_H( int x, int y);

/*
    @brief Draws the letter I at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_I( int x, int y);

/*
    @brief Draws the letter J at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_J( int x, int y);

/*
    @brief Draws the letter K at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_K( int x, int y);

/*
    @brief Draws the letter L at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_L( int x, int y);

/*
    @brief Draws the letter M at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_M( int x, int y);

/*
    @brief Draws the letter N at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_N( int x, int y);

/*
    @brief Draws the letter O at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_O( int x, int y);

/*
    @brief Draws the letter P at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_P( int x, int y);

/*
    @brief Draws the letter Q at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_Q( int x, int y);

/*
    @brief Draws the letter R at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_R( int x, int y);

/*
    @brief Draws the letter S at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_S( int x, int y);

/*
    @brief Draws the letter T at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_T( int x, int y);

/*
    @brief Draws the letter U at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_U( int x, int y);

/*
    @brief Draws the letter V at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_V( int x, int y);

/*
    @brief Draws the letter W at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_W( int x, int y);

/*
    @brief Draws the letter X at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_X( int x, int y);

/*
    @brief Draws the letter Y at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_Y( int x, int y);

/*
    @brief Draws the letter Z at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_Z( int x, int y);

//Functions for indicating the correctness of the user guesses

/*
    @brief Draws the black symbol at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_black(float x, float y);

/*
    @brief Draws the yellow symbol at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_yellow(float x, float y);

/*
    @brief Draws the green symbol at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_green(float x, float y);
#endif