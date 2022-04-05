#ifndef FUNCTIONS_LOADED
#define FUNCTIONS_LOADED

//Handles the drawing logic, inverse kinematics and shape functions.

/*
    @brief Moves the arms to the corresponding angles.
    @param theta_1 The target angle for servo 1
    @param theta_2 The target angle for servo 2
*/
void move(float theta_1, float theta_2);

/*
    @brief Moves the arms to the corresponding x and y coordinates.
    @param x_coor The target x coordinate
    @param y_coor The target y coordinate
*/
void move_xy(float x_coor, float y_coor);

/*
    @brief Moves the arms to the corresponding x and y coordinates with the pen lifted.
    @param x_coor The target x coordinate
    @param y_coor The target y coordinate
*/
void move_xy_with_lift(float x_coor, float y_coor);

/*
    @brief Checks if the given coordinates are within the bounds of the board.
    @param x_coor The x coordinate to be checked
    @param y_coor The y coordinate to be checked
    @return Whether or not the coordinates are within the bounds of the board
*/
bool within_bounds(float x_coor, float y_coor);

/*
    @brief Moves the pen in a lineair bezier curve (= a straight line).
    @param start_x The x coordinate of the start point
    @param start_y The y coordinate of the start point
    @param end_x The x coordinate of the end point
    @param end_y The y coordinate of the end point
*/
void lin_bez(float start_x, float start_y, float end_x, float end_y);
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
void cub_bez(float start_x, float start_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float end_x, float end_y);


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
void draw_letter(char letter, float x, float y);

//Functions for individual letters

/*
    @brief Draws the letter A at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_A(float x, float y);

/*
    @brief Draws the letter B at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_B(float x, float y);

/*
    @brief Draws the letter C at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_C(float x, float y);

/*
    @brief Draws the letter D at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_D(float x, float y);

/*
    @brief Draws the letter E at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_E(float x, float y);

/*
    @brief Draws the letter F at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_F(float x, float y);

/*
    @brief Draws the letter G at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_G(float x, float y);

/*
    @brief Draws the letter H at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_H(float x, float y);

/*
    @brief Draws the letter I at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_I(float x, float y);

/*
    @brief Draws the letter J at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_J(float x, float y);

/*
    @brief Draws the letter K at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_K(float x, float y);

/*
    @brief Draws the letter L at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_L(float x, float y);

/*
    @brief Draws the letter M at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_M(float x, float y);

/*
    @brief Draws the letter N at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_N(float x, float y);

/*
    @brief Draws the letter O at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_O(float x, float y);

/*
    @brief Draws the letter P at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_P(float x, float y);

/*
    @brief Draws the letter Q at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_Q(float x, float y);

/*
    @brief Draws the letter R at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_R(float x, float y);

/*
    @brief Draws the letter S at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_S(float x, float y);

/*
    @brief Draws the letter T at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_T(float x, float y);

/*
    @brief Draws the letter U at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_U(float x, float y);

/*
    @brief Draws the letter V at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_V(float x, float y);

/*
    @brief Draws the letter W at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_W(float x, float y);

/*
    @brief Draws the letter X at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_X(float x, float y);

/*
    @brief Draws the letter Y at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_Y(float x, float y);

/*
    @brief Draws the letter Z at the given coordinates (lower left corner of the letter's 'box')
    @param x The x-coordinate for the origin of the letter
    @param y The y-coordinate for the origin of the letter
*/
void draw_Z(float x, float y);

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