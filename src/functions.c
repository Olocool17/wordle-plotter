#include <stdbool.h>
#include <util/delay.h>
#include <math.h>
#include <assert.h>

#include <functions.h>
#include <servo.h>
#include <ui.h>

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//functions.c:
//Calculations done assuming L1 (= length of the first servo) = L2 (= length of second servo) = 13.8cm, with the coordinate system in increments of 0.1mm.
//No out of bounds check is needed for move_xy since boundaries were already checked during the bezier call, requiring only 2 or 4 checks instead of 100.
//The boundaries are those of an A5-sheet, being 15x21cm (rounded up)
//For the individual letters, the following formulas apply:
//value of x-position = (0.8 + 2*x_value/250) + 2.8*x
//value of y-position = (18 + 2*y_value/250) - 2.8*y
//the variables x and y are the discrete coordinates of the tiles on the 5x6 wordle grid, with (0,0) being the tile in the TOP left corner!
//the variables x_value and y_value are the x and y-values within an individual gridspace, both on a scale of 250 (each gridspace is 2cmx2cm in size)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//TODO:
//1) Test every letter in a position that checks all variables.
//2) Add pen-lifting function with corresponding dutycycle.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

//Bounds of the board

#define XMIN (0)
#define YMIN (300)
#define XMAX (1400)
#define YMAX (2100)

#define ARM_LENGTH (1380) //Length of each servo arm

#define MICROS_PI 2300 //The servo duty cycle in microseconds that corresponds to 180 degrees or pi radians

#define ITERS (0.06) //Amount of subdivisions for each 0.1mm of a curve
#define ITER_DELAY (10) //Time in miliseconds for the pen to trace each such subdivision

//Macro's for x² and x³
#define SQUARE(x) ((x)*(x))
#define CUBE(x) ((x)*(x)*(x))

int radians_to_micros(float rad)
{
    return 700 + (int)((rad / M_PI) * 1600);
}

void move(int theta_1, int theta_2) 
{
    servo1_dutymicros = theta_1;
    servo2_dutymicros = theta_2;
}

void move_xy(int x_coor, int y_coor) 
{ 
    int r = sqrt(SQUARE(x_coor) + SQUARE(y_coor));
    int help_theta = 2 * radians_to_micros(asin(r / (ARM_LENGTH * 2)));
    int theta_2 = MICROS_PI - help_theta;
    int theta_1 = (MICROS_PI / 2) - radians_to_micros(asin(y_coor / r)) + (help_theta / 2);
    move(theta_1, theta_2);
}

void move_xy_with_lift(int x_coor, int y_coor) 
{ 
    _delay_ms(150);
    //"lift pen"
    _delay_ms(150);
    move_xy(x_coor, y_coor);
    _delay_ms(150);
    //"lower pen"
    _delay_ms(150);
}

bool within_bounds(int x_coor, int y_coor) 
{
    if ((x_coor <  XMIN)|| (x_coor > XMAX) || (y_coor < YMIN) || (y_coor > YMAX)) 
    {
        return false;
    }
    return true;
}

void lin_bez(int start_x, int start_y, int end_x, int end_y)  
{
    if (!(within_bounds(start_x,start_y) && within_bounds(end_x,end_y))) return;
    int pathlength = sqrt(SQUARE(end_x - start_x) + SQUARE(end_y - start_y));
    int segments = (int) (ITERS * pathlength);
    if (segments == 0) return; //too small to draw at current precision
    int segment_x = (end_x - start_x)/segments;
    int segment_y = (end_y - start_y)/segments;
    int x_next = start_x;
    int y_next = start_y;
    move_xy(x_next, y_next);
    for(size_t t = 0; t < segments ; t++) 
    {
        x_next += segment_x;
        y_next += segment_y;
        _delay_ms(ITER_DELAY);
        move_xy(x_next, y_next); //transfer next coordinates to the angle finding function
    }
}

void cub_bez(int start_x, int start_y, int cp1_x, int cp1_y, int cp2_x, int cp2_y, int end_x, int end_y) {
    if (!((within_bounds(start_x,start_y) && within_bounds(cp1_x, cp1_y) && within_bounds(cp2_x, cp2_y) && within_bounds(end_x, end_y)))) return;
    int pathlength = sqrt(SQUARE(end_x - start_x) + SQUARE(end_y - start_y));
    int segments = (int) (ITERS * pathlength);
    if (segments == 0) return; //too small to draw at current precision
    for (size_t t = 0; t <= segments; t++)
    {
        int x_next = (CUBE(segments - t)*start_x + 3*SQUARE(segments - t)*t*cp1_x + 3*(segments - t)*SQUARE(t)*cp2_x + CUBE( t)*end_x) / CUBE(segments);
        int y_next = (CUBE(segments - t)*start_y + 3*SQUARE(segments - t)*t*cp1_y + 3*(segments - t)*SQUARE(t)*cp2_y + CUBE( t)*end_y) / CUBE(segments);
        _delay_ms(ITER_DELAY);
        move_xy(x_next, y_next); //transfer next coordinates to the angle finding function
    } 
}

void draw_grid()
{
    drawing();
    for (size_t i = XMIN; i < XMAX; i++) 
    {
        move_xy_with_lift(i, YMIN);
        lin_bez(i, YMIN, i, YMAX);
    }
    for (size_t i = YMIN; i < YMAX; i++)
    {
        move_xy_with_lift(XMIN, i);
        lin_bez(XMIN, i, XMAX, i);
    }
    
}

void draw_letter(char letter, int x, int y)
{
    drawing();
    switch(letter) 
    {
        case 'A':
            draw_A(x, y);
            break;
        case 'B':
            draw_B(x, y);
            break;
        case 'C':
            draw_C(x, y);
            break;
        case 'D':
            draw_D(x, y);
            break;
        case 'E':
            draw_E(x, y);
            break;
        case 'F':
            draw_F(x, y);
            break;
        case 'G':
            draw_G(x, y);
            break;
        case 'H':
            draw_H(x, y);
            break;
        case 'I':
            draw_I(x, y);
            break;
        case 'J':
            draw_J(x, y);
            break;
        case 'K':
            draw_K(x, y);
            break;
        case 'L':
            draw_L(x, y);
            break;
        case 'M':
            draw_M(x, y);
            break;
        case 'N':
            draw_N(x, y);
            break;
        case 'O':
            draw_O(x, y);
            break;
        case 'P':
            draw_P(x, y);
            break;
        case 'Q':
            draw_Q(x, y);
            break;
        case 'R':
            draw_R(x, y);
            break;
        case 'S':
            draw_S(x, y);
            break;
        case 'T':
            draw_T(x, y);
            break;
        case 'U':
            draw_U(x, y);
            break;
        case 'V':
            draw_V(x, y);
            break;
        case 'W':
            draw_W(x, y);
            break;
        case 'X':
            draw_X(x, y);
            break;
        case 'Y':
            draw_Y(x, y);
            break;
        case 'Z':
            draw_Z(x, y);
            break;
        case 'g':
            draw_green(x, y);
        case 'y':
            draw_yellow(x, y);
        case 'b':
            draw_black(x, y);
        default:
            return;
    }
}

void draw_letter_on_grid(char letter, int tile_x, int tile_y)
{
    int x = 100 + 260 * tile_x; //letters are bound between 100 < x < 1200
    int y = 1800 - 300 * tile_y; //letters are bound between 600 < y < 2000
    draw_letter(letter, x, y);
}

void draw_A( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x + 80, y + 200);
    lin_bez(x + 80, y + 200, x + 160, y);
    move_xy_with_lift(x + 40, y + 100);
    lin_bez(x + 40, y + 100, x + 120, y + 100);
}

void draw_B( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x, y + 200);
    cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
    cub_bez(x, y + 100, x + 200, y + 100, x + 200, y, x, y );
}

void draw_C( int x, int y) 
{
    move_xy_with_lift(x + 180, y);
    cub_bez(x + 160, y + 20, x - 50, y - 50, x - 50, y + 250, x + 160, y + 180);
}

void draw_D( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x, y + 200);
    cub_bez(x, y + 200, x + 200, y + 200, x + 200, y, x, y);
}

void draw_E( int x, int y) 
{
    move_xy_with_lift(x + 160, y);
    lin_bez(x + 160, y, x, y);
    lin_bez(x, y, x, y + 200);
    lin_bez(x, y + 200, x + 160, y + 200);
    move_xy_with_lift(x, y + 100);
    lin_bez(x, y + 100, x + 120, y + 100);
}

void draw_F( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x, y + 200);
    lin_bez(x, y + 200, x + 160, y + 200);
    move_xy_with_lift(x, y + 100);
    lin_bez(x, y + 100, x + 120, y + 100);
}

void draw_G( int x, int y) 
{
    move_xy_with_lift(x + 100, y + 100);
    lin_bez(x + 100, y + 100, x + 160, y + 100);
    cub_bez(x + 160, y + 100, x + 160, y - 30, x, y - 30, x, y + 100);
    cub_bez(x, y + 100, x, y + 230, x + 160, y + 200, x + 160, y + 170);

}

void draw_H( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x, y + 200);
    move_xy_with_lift(x, y + 120);
    lin_bez(x, y + 120, x + 160, y + 120);
    move_xy_with_lift(x + 160, y);
    lin_bez(x + 160, y, x + 160, y + 200);
}

void draw_I( int x, int y) 
{
    move_xy_with_lift(x + 40, y);
    lin_bez(x + 40, y, x + 120, y);
    move_xy_with_lift(x + 80, y);
    lin_bez(x + 80, y, x + 80, y + 200);
    move_xy_with_lift(x + 40, y + 200);
    lin_bez(x + 40, y + 200, x + 120, y + 200);
}

void draw_J( int x, int y) 
{
    move_xy_with_lift(x + 80, y + 200);
    lin_bez(x + 80, y + 200, x + 160, y + 200);
    move_xy_with_lift(x + 120, y + 200);
    lin_bez(x + 120, y + 200, x + 120, y + 100);
    cub_bez(x + 120, y + 100, x + 120, y - 30, x, y - 30, x, y + 100);
}

void draw_K( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x, y + 200);
    move_xy_with_lift(x + 160, y);
    lin_bez(x + 160, y, x, y + 100);
    lin_bez(x, y + 100, x + 160, y + 200);
}

void draw_L( int x, int y) 
{
    move_xy_with_lift(x, y + 200);
    lin_bez(x, y + 200, x, y);
    lin_bez(x, y, x + 160, y);
}

void draw_M( int x, int y) 
{
    move_xy_with_lift(x,y);
    lin_bez(x, y, x, y + 200);
    lin_bez(x, y + 200, x + 80, y + 120);
    lin_bez(x + 80, y + 120, x + 160, y + 200);
    lin_bez(x + 160, y + 200, x + 160, y);
}

void draw_N( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x, y + 200);
    lin_bez(x, y + 200, x + 160, y);
    lin_bez(x + 160, y, x + 160, y + 200);
}

void draw_O( int x, int y) 
{
    move_xy_with_lift(x, y + 100);
    cub_bez(x, y + 100, x, y + 230, x + 160, y + 230, x + 160, y + 100);
    cub_bez(x + 160, y + 100, x + 160, y - 30, x, y - 30, x, y + 100);
}

void draw_P( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x, y + 200);
    cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
}

void draw_Q( int x, int y) 
{
    move_xy_with_lift(x + 80, y + 80);
    lin_bez(x + 80, y + 80, x + 160, y);
    draw_O(x, y);
}

void draw_R( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x, y + 200);
    cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
    lin_bez(x, y + 100, x + 160, y);
}

void draw_S( int x, int y) 
{
    move_xy_with_lift(x, y);
    lin_bez(x, y, x + 80, y);
    cub_bez(x + 80, y, x + 160, y, x + 160, y + 100, x + 80, y + 100);
    cub_bez(x + 80, y + 100, x, y + 100, x, y + 200, x + 80, y + 200);
    lin_bez(x + 80, y + 200, x + 160, y + 200);
}

void draw_T( int x, int y) 
{
    move_xy_with_lift(x, y + 200);
    lin_bez(x, y + 200, x + 160, y + 200);
    move_xy_with_lift(x + 80, y + 200);
    lin_bez(x + 80, y + 200, x + 80, 0);
}

void draw_U( int x, int y) 
{
    move_xy_with_lift(x, y + 200);
    lin_bez(x, y + 200, x, y + 100);
    cub_bez(x, y + 100, x, y - 30, x + 160, y - 30, x + 160, y + 100);
    lin_bez(x + 160, y + 100, x + 160, y + 200);
}

void draw_V( int x, int y) 
{
    move_xy_with_lift(x, y + 200);
    lin_bez(x, y + 200, x + 80, y);
    lin_bez(x + 80, y, x + 160, y + 200);
}

void draw_W( int x, int y) 
{
    move_xy_with_lift(x, y + 200);
    lin_bez(x, y + 200, x, y);
    lin_bez(x, y, x + 80, y + 80);
    lin_bez(x + 80, y + 80, x + 160, y);
    lin_bez(x + 160, y, x + 160, y + 200);
}

void draw_X( int x, int y) 
{
    move_xy_with_lift(x, y + 200);
    lin_bez(x, y + 200, x + 160, y);
    move_xy_with_lift(x, y);
    lin_bez(x, y, x + 160, y + 200);
}

void draw_Y( int x, int y) 
{
    move_xy_with_lift(x, y + 200);
    lin_bez(x, y + 200, x + 80, y + 100);
    lin_bez(x + 80, y + 100, x + 160, y + 200);
    move_xy_with_lift(x + 80, y + 100);
    lin_bez(x + 80, y + 100, x + 80, y);
}

void draw_Z( int x, int y) 
{
    move_xy_with_lift(x, y + 200);
    lin_bez(x, y + 200, x + 160, y + 200);
    lin_bez(x + 160, y + 200, x, y);
    lin_bez(x, y, x + 160, y);
}

void draw_black(float x, float y) 
{
    move_xy_with_lift(x + 110, y + 250);
    lin_bez(x + 110, y + 250, x + 150, y + 210);
    move_xy_with_lift(x + 110, y + 210);
    lin_bez(x + 110, y + 210, x + 150, y + 250);
}

void draw_yellow(float x, float y) 
{
    move_xy_with_lift(x + 88, y + 225);
    cub_bez(x + 88, y + 225, x + 122, y + 270, x + 122, y + 180, x + 161, y + 237);
}

void draw_green(float x, float y) 
{
    move_xy_with_lift(x + 110, y + 250);
    lin_bez(x + 110, y + 250, x + 130, y + 210);
    lin_bez(x + 130, y + 210, x + 150, y + 250);
}