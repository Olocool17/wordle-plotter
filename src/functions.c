#include <stdbool.h>
#include <util/delay.h>
#include <math.h>
#include <assert.h>

#include <functions.h>
#include <servo.h>
#include <ui.h>

/* -----------------------------------------------------------------------------------------------------------------------------------------------------------------
functions.c:
Calculations done assuming L1 (= length of the first servo) = L2 (= length of second servo) = 13.8cm, with the coordinate system in increments of 0.1mm.
No out of bounds check is needed for move_xy since boundaries were already checked during the bezier call, requiring only 2 or 4 checks instead of 100.
The boundaries are 14x21cm, roughly the size of an A5 paper
------------------------------------------------------------------------------------------------------------------------------------------------------------------
TODO:
1) Test every letter in a position that checks all variables.
2) Add pen-lifting function with corresponding dutycycle.
3) Change grid to actually account for 6 attempts :(
----------------------------------------------------------------------------------------------------------------------------------------------------------------- */

//Bounds of the board

#define XMIN (0)
#define YMIN (300)
#define XMAX (1400)
#define YMAX (2100)

//Gridspace measurements

#define LETTER_WIDTH (260)
#define LETTER_HEIGHT (300)

#define ARM_LENGTH_1 (1250) //Length of each servo arm
#define ARM_LENGTH_2 (1450)

#define MICROS_PI (1600) //The servo duty cycle difference in microseconds that corresponds to 180 degrees or pi radians
#define PWM_BEGIN (700)
#define PWM_END (2300)

#define ITERS (0.06) //Amount of subdivisions for each 0.1mm of a curve
#define ITER_DELAY (10) //Time in miliseconds for the pen to trace each such subdivision

//Macro's for x² and x³
#define SQUARE(x) ((x)*(x))
#define CUBE(x) ((x)*(x)*(x))

int radians_to_micros(float rad)
{
    return ((rad / M_PI) * MICROS_PI);
}

void move(int theta_1, int theta_2) 
{
    servo1_dutymicros = PWM_BEGIN + theta_1;
    servo2_dutymicros = PWM_BEGIN + theta_2 - (80*(3*CUBE(theta_1) + 8*CUBE(theta_1-800) + 22*CUBE(theta_1-1600)))/(9*(theta_1*(theta_1-800)*(theta_1-1600)));
}

void move_xy(int x_coor, int y_coor) 
{ 
    float r = sqrt(SQUARE((long)x_coor) + SQUARE((long) y_coor));
    float help_theta = acos((float)(SQUARE((long)ARM_LENGTH_1) + SQUARE((long)ARM_LENGTH_2) - SQUARE(r)) / (2 * (long)ARM_LENGTH_1 * (long)ARM_LENGTH_2));
    int theta_1 = MICROS_PI - radians_to_micros(atan2(y_coor, x_coor)) - radians_to_micros(asin(ARM_LENGTH_2 * sin(help_theta) / r));
    int theta_2 = MICROS_PI - radians_to_micros(help_theta); 
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
    if ((x_coor <  XMIN) || (x_coor > XMAX) || (y_coor < YMIN) || (y_coor > YMAX)) 
    {
        return false;
    }
    return true;
}

void lin_bez(int start_x, int start_y, int end_x, int end_y)  
{
    if (!(within_bounds(start_x,start_y) && within_bounds(end_x,end_y))) return; //given values out-of-bounds
    
    int pathlength = sqrt(SQUARE((long)end_x - (long)start_x) + SQUARE((long)end_y - (long)start_y));
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
        //formula for the Cubic Bezier curve
        int x_next = (CUBE(segments - t)*start_x + 3*SQUARE(segments - t)*t*cp1_x + 3*(segments - t)*SQUARE(t)*cp2_x + CUBE( t)*end_x) / CUBE(segments);
        int y_next = (CUBE(segments - t)*start_y + 3*SQUARE(segments - t)*t*cp1_y + 3*(segments - t)*SQUARE(t)*cp2_y + CUBE( t)*end_y) / CUBE(segments);
        _delay_ms(ITER_DELAY);
        move_xy(x_next, y_next); //transfer next coordinates to the angle finding function
    } 
}

void draw_grid()
{
    drawing();
    for (size_t i = XMIN; i < XMAX; i += 100) 
    {
        move_xy_with_lift(i, YMIN);
        lin_bez(i, YMIN, i, YMAX);
    }
    for (size_t i = YMIN; i < YMAX; i += 100)
    {
        move_xy_with_lift(XMIN, i);
        lin_bez(XMIN, i, XMAX, i);
    }
    
}

//drawing letters on the grid
void draw_letter(char letter, int x, int y)
{
    drawing();
    switch(letter) 
    {
        case 'A':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x + 80, y + 200);
            lin_bez(x + 80, y + 200, x + 160, y);
            move_xy_with_lift(x + 40, y + 100);
            lin_bez(x + 40, y + 100, x + 120, y + 100);
            break;
        case 'B':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
            cub_bez(x, y + 100, x + 200, y + 100, x + 200, y, x, y );
            break;
        case 'C':
            move_xy_with_lift(x + 180, y);
            cub_bez(x + 160, y + 20, x - 50, y - 50, x - 50, y + 250, x + 160, y + 180);
            break;
        case 'D':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            cub_bez(x, y + 200, x + 200, y + 200, x + 200, y, x, y);
            break;
        case 'E':
            move_xy_with_lift(x + 160, y);
            lin_bez(x + 160, y, x, y);
            lin_bez(x, y, x, y + 200);
            lin_bez(x, y + 200, x + 160, y + 200);
            move_xy_with_lift(x, y + 100);
            lin_bez(x, y + 100, x + 120, y + 100);
            break;
        case 'F':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            lin_bez(x, y + 200, x + 160, y + 200);
            move_xy_with_lift(x, y + 100);
            lin_bez(x, y + 100, x + 120, y + 100);
            break;
        case 'G':
            move_xy_with_lift(x + 100, y + 100);
            lin_bez(x + 100, y + 100, x + 160, y + 100);
            cub_bez(x + 160, y + 100, x + 160, y - 30, x, y - 30, x, y + 100);
            cub_bez(x, y + 100, x, y + 230, x + 160, y + 200, x + 160, y + 170);
            break;
        case 'H':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            move_xy_with_lift(x, y + 120);
            lin_bez(x, y + 120, x + 160, y + 120);
            move_xy_with_lift(x + 160, y);
            lin_bez(x + 160, y, x + 160, y + 200);
            break;
        case 'I':
            move_xy_with_lift(x + 40, y);
            lin_bez(x + 40, y, x + 120, y);
            move_xy_with_lift(x + 80, y);
            lin_bez(x + 80, y, x + 80, y + 200);
            move_xy_with_lift(x + 40, y + 200);
            lin_bez(x + 40, y + 200, x + 120, y + 200);
            break;
        case 'J':
            move_xy_with_lift(x + 80, y + 200);
            lin_bez(x + 80, y + 200, x + 160, y + 200);
            move_xy_with_lift(x + 120, y + 200);
            lin_bez(x + 120, y + 200, x + 120, y + 100);
            cub_bez(x + 120, y + 100, x + 120, y - 30, x, y - 30, x, y + 100);
            break;
        case 'K':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            move_xy_with_lift(x + 160, y);
            lin_bez(x + 160, y, x, y + 100);
            lin_bez(x, y + 100, x + 160, y + 200);
            break;
        case 'L':
            move_xy_with_lift(x, y + 200);
            lin_bez(x, y + 200, x, y);
            lin_bez(x, y, x + 160, y);
            break;
        case 'M':
            move_xy_with_lift(x,y);
            lin_bez(x, y, x, y + 200);
            lin_bez(x, y + 200, x + 80, y + 120);
            lin_bez(x + 80, y + 120, x + 160, y + 200);
            lin_bez(x + 160, y + 200, x + 160, y);
            break;
        case 'N':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            lin_bez(x, y + 200, x + 160, y);
            lin_bez(x + 160, y, x + 160, y + 200);
            break;
        case 'O':
            move_xy_with_lift(x, y + 100);
            cub_bez(x, y + 100, x, y + 230, x + 160, y + 230, x + 160, y + 100);
            cub_bez(x + 160, y + 100, x + 160, y - 30, x, y - 30, x, y + 100);
            break;
        case 'P':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
            break;
        case 'Q':
            move_xy_with_lift(x + 80, y + 80);
            lin_bez(x + 80, y + 80, x + 160, y);
            draw_letter('O', x, y);
            break;
        case 'R':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
            lin_bez(x, y + 100, x + 160, y);
            break;
        case 'S':
            move_xy_with_lift(x, y);
            lin_bez(x, y, x + 80, y);
            cub_bez(x + 80, y, x + 160, y, x + 160, y + 100, x + 80, y + 100);
            cub_bez(x + 80, y + 100, x, y + 100, x, y + 200, x + 80, y + 200);
            lin_bez(x + 80, y + 200, x + 160, y + 200);
            break;
        case 'T':
            move_xy_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 160, y + 200);
            move_xy_with_lift(x + 80, y + 200);
            lin_bez(x + 80, y + 200, x + 80, 0);
            break;
        case 'U':
            move_xy_with_lift(x, y + 200);
            lin_bez(x, y + 200, x, y + 100);
            cub_bez(x, y + 100, x, y - 30, x + 160, y - 30, x + 160, y + 100);
            lin_bez(x + 160, y + 100, x + 160, y + 200);
            break;
        case 'V':
            move_xy_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 80, y);
            lin_bez(x + 80, y, x + 160, y + 200);
            break;
        case 'W':
            move_xy_with_lift(x, y + 200);
            lin_bez(x, y + 200, x, y);
            lin_bez(x, y, x + 80, y + 80);
            lin_bez(x + 80, y + 80, x + 160, y);
            lin_bez(x + 160, y, x + 160, y + 200);
            break;
        case 'X':
            move_xy_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 160, y);
            move_xy_with_lift(x, y);
            lin_bez(x, y, x + 160, y + 200);
            break;
        case 'Y':
            move_xy_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 80, y + 100);
            lin_bez(x + 80, y + 100, x + 160, y + 200);
            move_xy_with_lift(x + 80, y + 100);
            lin_bez(x + 80, y + 100, x + 80, y);
            break;
        case 'Z':
            move_xy_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 160, y + 200);
            lin_bez(x + 160, y + 200, x, y);
            lin_bez(x, y, x + 160, y);
            break;
        case 'g': //draws a small "v", corresponds to a green tile from the original game
            move_xy_with_lift(x + 110, y + 250);
            lin_bez(x + 110, y + 250, x + 130, y + 210);
            lin_bez(x + 130, y + 210, x + 150, y + 250);
        case 'y': //draws a small "~", corresponds to a yellow tile from the original game
            move_xy_with_lift(x + 88, y + 225);
            cub_bez(x + 88, y + 225, x + 122, y + 270, x + 122, y + 180, x + 161, y + 237);
        case 'b': //draws a small "x", corresponds to a black (empty) tile from the original game
            move_xy_with_lift(x + 110, y + 250);
            lin_bez(x + 110, y + 250, x + 150, y + 210);
            move_xy_with_lift(x + 110, y + 210);
            lin_bez(x + 110, y + 210, x + 150, y + 250);
        default:
            return;
    }
}

void draw_letter_on_grid(char letter, int tile_x, int tile_y)
{
    int x = 100 + LETTER_WIDTH * tile_x; //letters are bound between 100 < x < 1200
    int y = 2100 - LETTER_HEIGHT * tile_y; //letters are bound between 600 < y < 2000
    draw_letter(letter, x, y);
}