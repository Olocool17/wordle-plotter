#include <stdbool.h>
#include <util/delay.h>
#include <math.h>
#include <assert.h>

#include <functions.h>
#include <servo.h>
#include <ui.h>

//Bounds of the board

#define XMIN (600)
#define YMIN (0)
#define XMAX (2600)
#define YMAX (1700)

//Gridspace measurements

#define LETTER_WIDTH (260)
#define LETTER_HEIGHT (300)

#define ARM_LENGTH_1 (1250) //Length of each servo arm
#define ARM_LENGTH_2 (1800)

#define MICROS_PI (1600) //The servo duty cycle difference in microseconds that corresponds to 180 degrees or pi radians
#define PWM_BEGIN (700)
#define PWM_END (2300)

#define LIFT_DELAY (80) //Amount of wait time when lifting / dropping the pen before proceeding with movement

#define ITERS (0.1) //Amount of subdivisions for each 0.1mm of a curve
#define ITER_DELAY (10) //Time in miliseconds for the pen to trace each such subdivision

//Primitive constants

#define CIRCLE_ITERS (10)
#define CIRCLE_RADIUS (300)
#define CIRCLE_CENTER_X (XMIN + (XMAX - XMIN) / 2)
#define CIRCLE_CENTER_Y (YMIN + (YMAX - YMIN) / 2)

#define SQUARE_SIDE (500)
#define SQUARE_CENTER_X (XMIN + (XMAX - XMIN) / 2)
#define SQUARE_CENTER_Y (YMIN + (YMAX - YMIN) / 2)

int current_x = CIRCLE_CENTER_X;
int current_y = CIRCLE_CENTER_Y;

int radians_to_micros(float rad)
{
    return ((rad / M_PI) * MICROS_PI);
}

void move(int theta_1, int theta_2) 
{
    servo1_dutymicros = PWM_BEGIN + theta_1 + (int)((17/(double)256000)*pow((long)theta_1, 2) + (long)theta_1/(float)320 + 25); //Converts naive PWM duty cycles to calibrated ones
    servo2_dutymicros = PWM_BEGIN + theta_2 + (int)((51/(double)1280000)*pow((long)theta_2, 2) + (93/(float)1600)*(long)theta_2 - 133);
}

void move_xy(int x_coor, int y_coor) 
{ 
    float r = sqrt(pow((long)x_coor, 2) + pow((long) y_coor, 2));
    float help_theta = acos((float)(pow((long)ARM_LENGTH_1, 2) + pow((long)ARM_LENGTH_2, 2) - pow(r, 2)) / (2 * (long)ARM_LENGTH_1 * (long)ARM_LENGTH_2));
    int theta_1 = MICROS_PI + (MICROS_PI / 2) - radians_to_micros(atan2(y_coor, x_coor)) - radians_to_micros(acos((float)(pow((long)ARM_LENGTH_1, 2) + pow(r, 2) - pow((long)ARM_LENGTH_2, 2)) / (2 * (long)ARM_LENGTH_1 * r)));
    int theta_2 = MICROS_PI - radians_to_micros(help_theta); 
    move(theta_1, theta_2);
    current_x = x_coor;
    current_y = y_coor;
}

bool within_bounds(int x_coor, int y_coor) 
{
    if ((x_coor <  XMIN) || (x_coor > XMAX) || (y_coor < YMIN) || (y_coor > YMAX)) 
    {
        return false;
    }
    return true;
}

void lin_bez_with_lift(int end_x, int end_y)
{
    for (servo3_dutymicros = 1000; servo3_dutymicros < 2000; servo3_dutymicros += 50) 
    {
        _delay_ms(LIFT_DELAY);
    }
    lin_bez(current_x, current_y, end_x, end_y);
    for (servo3_dutymicros = 2000; servo3_dutymicros > 1000; servo3_dutymicros -= 50) 
    {
        _delay_ms(LIFT_DELAY);
    }
}

void lin_bez(int start_x, int start_y, int end_x, int end_y)  
{
    if (!(within_bounds(start_x,start_y) && within_bounds(end_x,end_y))) return; //given values out-of-bounds
    
    int pathlength = sqrt(pow((long)end_x - (long)start_x, 2) + pow((long)end_y - (long)start_y, 2));
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
    if (!((within_bounds(start_x,start_y) && within_bounds(end_x, end_y)))) return;
    int pathlength = sqrt(pow((long)cp1_x - (long)start_x, 2) + pow((long)cp1_y - (long)start_y, 2));
    pathlength += sqrt(pow((long)cp2_x - (long)cp1_x, 2) + pow((long)cp2_y - (long)cp1_y, 2));
    pathlength += sqrt(pow((long)end_x - (long)cp2_x, 2) + pow((long)end_y - (long)cp2_y, 2));
    int segments = (int) (ITERS * pathlength);
    if (segments == 0) return; //too small to draw at current precision
    for (float u = 0; u <= 1.0; u += 1.0/(float)segments)
    {
        //formula for the Cubic Bezier curve
        int x_next = pow(1-u, 3)*start_x + 3*u*pow(1-u, 2)*cp1_x + 3*pow(u, 2)*(1-u)*cp2_x + pow(u, 3)*end_x;
        int y_next = pow(1-u, 3)*start_y + 3*u*pow(1-u, 2)*cp1_y + 3*pow(u, 2)*(1-u)*cp2_y + pow(u, 3)*end_y;
        _delay_ms(ITER_DELAY);
        move_xy(x_next, y_next); //transfer next coordinates to the angle finding function
    } 
}

void draw_grid()
{
    drawing();
    for (size_t i = XMIN; i < XMAX; i += 100) 
    {
        lin_bez_with_lift(i, YMIN);
        lin_bez(i, YMIN, i, YMAX);
    }
    for (size_t i = YMIN; i < YMAX; i += 100)
    {
        lin_bez_with_lift(XMIN, i);
        lin_bez(XMIN, i, XMAX, i);
    }
    
}

void draw_circle()
{
    drawing();
    lin_bez_with_lift(CIRCLE_CENTER_X + CIRCLE_RADIUS, CIRCLE_CENTER_Y);
    float circle_subdiv = 2 * M_PI / CIRCLE_ITERS;
    float new_i = 0;
    for (float i = 0 ; i <= 2 * M_PI;)
    {
        new_i += circle_subdiv;
        lin_bez(CIRCLE_CENTER_X + CIRCLE_RADIUS * cos(i), CIRCLE_CENTER_Y + CIRCLE_RADIUS*sin(i) , CIRCLE_CENTER_X + CIRCLE_RADIUS * cos(new_i), CIRCLE_CENTER_Y + CIRCLE_RADIUS * sin(new_i));
        i = new_i;
    }
}

void draw_square()
{
    drawing();
    lin_bez_with_lift(SQUARE_CENTER_X - SQUARE_SIDE / 2, SQUARE_CENTER_Y - SQUARE_SIDE / 2);
    lin_bez(SQUARE_CENTER_X - SQUARE_SIDE / 2, SQUARE_CENTER_Y - SQUARE_SIDE / 2, SQUARE_CENTER_X - SQUARE_SIDE / 2, SQUARE_CENTER_Y + SQUARE_SIDE / 2);
    lin_bez(SQUARE_CENTER_X - SQUARE_SIDE / 2, SQUARE_CENTER_Y + SQUARE_SIDE / 2, SQUARE_CENTER_X + SQUARE_SIDE / 2, SQUARE_CENTER_Y + SQUARE_SIDE / 2);
    lin_bez(SQUARE_CENTER_X + SQUARE_SIDE / 2, SQUARE_CENTER_Y + SQUARE_SIDE / 2, SQUARE_CENTER_X + SQUARE_SIDE / 2, SQUARE_CENTER_Y - SQUARE_SIDE / 2);
    lin_bez(SQUARE_CENTER_X + SQUARE_SIDE / 2, SQUARE_CENTER_Y - SQUARE_SIDE / 2, SQUARE_CENTER_X - SQUARE_SIDE / 2, SQUARE_CENTER_Y - SQUARE_SIDE / 2);
}

//drawing letters on the grid
void draw_letter(char letter, int x, int y)
{
    drawing();
    switch(letter) 
    {
        case 'A':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x + 80, y + 200);
            lin_bez(x + 80, y + 200, x + 160, y);
            lin_bez_with_lift(x + 40, y + 100);
            lin_bez(x + 40, y + 100, x + 120, y + 100);
            break;
        case 'B':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
            cub_bez(x, y + 100, x + 200, y + 100, x + 200, y, x, y );
            break;
        case 'C':
            lin_bez_with_lift(x + 180, y);
            cub_bez(x + 160, y + 20, x - 50, y - 50, x - 50, y + 250, x + 160, y + 180);
            break;
        case 'D':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            cub_bez(x, y + 200, x + 200, y + 200, x + 200, y, x, y);
            break;
        case 'E':
            lin_bez_with_lift(x + 160, y);
            lin_bez(x + 160, y, x, y);
            lin_bez(x, y, x, y + 200);
            lin_bez(x, y + 200, x + 160, y + 200);
            lin_bez_with_lift(x, y + 100);
            lin_bez(x, y + 100, x + 120, y + 100);
            break;
        case 'F':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            lin_bez(x, y + 200, x + 160, y + 200);
            lin_bez_with_lift(x, y + 100);
            lin_bez(x, y + 100, x + 120, y + 100);
            break;
        case 'G':
            lin_bez_with_lift(x + 100, y + 100);
            lin_bez(x + 100, y + 100, x + 160, y + 100);
            cub_bez(x + 160, y + 100, x + 160, y - 30, x, y - 30, x, y + 100);
            cub_bez(x, y + 100, x, y + 230, x + 160, y + 200, x + 160, y + 170);
            break;
        case 'H':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            lin_bez_with_lift(x, y + 120);
            lin_bez(x, y + 120, x + 160, y + 120);
            lin_bez_with_lift(x + 160, y);
            lin_bez(x + 160, y, x + 160, y + 200);
            break;
        case 'I':
            lin_bez_with_lift(x + 40, y);
            lin_bez(x + 40, y, x + 120, y);
            lin_bez_with_lift(x + 80, y);
            lin_bez(x + 80, y, x + 80, y + 200);
            lin_bez_with_lift(x + 40, y + 200);
            lin_bez(x + 40, y + 200, x + 120, y + 200);
            break;
        case 'J':
            lin_bez_with_lift(x + 80, y + 200);
            lin_bez(x + 80, y + 200, x + 160, y + 200);
            lin_bez_with_lift(x + 120, y + 200);
            lin_bez(x + 120, y + 200, x + 120, y + 100);
            cub_bez(x + 120, y + 100, x + 120, y - 30, x, y - 30, x, y + 100);
            break;
        case 'K':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            lin_bez_with_lift(x + 160, y);
            lin_bez(x + 160, y, x, y + 100);
            lin_bez(x, y + 100, x + 160, y + 200);
            break;
        case 'L':
            lin_bez_with_lift(x, y + 200);
            lin_bez(x, y + 200, x, y);
            lin_bez(x, y, x + 160, y);
            break;
        case 'M':
            lin_bez_with_lift(x,y);
            lin_bez(x, y, x, y + 200);
            lin_bez(x, y + 200, x + 80, y + 120);
            lin_bez(x + 80, y + 120, x + 160, y + 200);
            lin_bez(x + 160, y + 200, x + 160, y);
            break;
        case 'N':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            lin_bez(x, y + 200, x + 160, y);
            lin_bez(x + 160, y, x + 160, y + 200);
            break;
        case 'O':
            lin_bez_with_lift(x, y + 100);
            cub_bez(x, y + 100, x, y + 230, x + 160, y + 230, x + 160, y + 100);
            cub_bez(x + 160, y + 100, x + 160, y - 30, x, y - 30, x, y + 100);
            break;
        case 'P':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
            break;
        case 'Q':
            lin_bez_with_lift(x + 80, y + 80);
            lin_bez(x + 80, y + 80, x + 160, y);
            draw_letter('O', x, y);
            break;
        case 'R':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x, y + 200);
            cub_bez(x, y + 200, x + 200, y + 200, x + 200, y + 100, x, y + 100);
            lin_bez(x, y + 100, x + 160, y);
            break;
        case 'S':
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x + 80, y);
            cub_bez(x + 80, y, x + 160, y, x + 160, y + 100, x + 80, y + 100);
            cub_bez(x + 80, y + 100, x, y + 100, x, y + 200, x + 80, y + 200);
            lin_bez(x + 80, y + 200, x + 160, y + 200);
            break;
        case 'T':
            lin_bez_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 160, y + 200);
            lin_bez_with_lift(x + 80, y + 200);
            lin_bez(x + 80, y + 200, x + 80, y);
            break;
        case 'U':
            lin_bez_with_lift(x, y + 200);
            lin_bez(x, y + 200, x, y + 100);
            cub_bez(x, y + 100, x, y - 30, x + 160, y - 30, x + 160, y + 100);
            lin_bez(x + 160, y + 100, x + 160, y + 200);
            break;
        case 'V':
            lin_bez_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 80, y);
            lin_bez(x + 80, y, x + 160, y + 200);
            break;
        case 'W':
            lin_bez_with_lift(x, y + 200);
            lin_bez(x, y + 200, x, y);
            lin_bez(x, y, x + 80, y + 80);
            lin_bez(x + 80, y + 80, x + 160, y);
            lin_bez(x + 160, y, x + 160, y + 200);
            break;
        case 'X':
            lin_bez_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 160, y);
            lin_bez_with_lift(x, y);
            lin_bez(x, y, x + 160, y + 200);
            break;
        case 'Y':
            lin_bez_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 80, y + 100);
            lin_bez(x + 80, y + 100, x + 160, y + 200);
            lin_bez_with_lift(x + 80, y + 100);
            lin_bez(x + 80, y + 100, x + 80, y);
            break;
        case 'Z':
            lin_bez_with_lift(x, y + 200);
            lin_bez(x, y + 200, x + 160, y + 200);
            lin_bez(x + 160, y + 200, x, y);
            lin_bez(x, y, x + 160, y);
            break;
        case 'g': //draws a small "v", corresponds to a green tile from the original game
            lin_bez_with_lift(x + 90, y + 270);
            lin_bez(x + 90, y + 270, x + 150, y + 190);
            lin_bez(x + 150, y + 190, x + 210, y + 270);
            break;
        case 'y': //draws a small "~", corresponds to a yellow tile from the original game
            lin_bez_with_lift(x + 50, y + 225);
            cub_bez(x + 50, y + 225, x + 122, y + 300, x + 122, y + 150, x + 200, y + 237);
            break;
        case 'b': //draws a small "x", corresponds to a black (empty) tile from the original game
            lin_bez_with_lift(x + 90, y + 270);
            lin_bez(x + 90, y + 270, x + 170, y + 190);
            lin_bez_with_lift(x + 90, y + 190);
            lin_bez(x + 110, y + 190, x + 170, y + 270);
            break;
        default:
            return;
    }
}

void draw_letter_on_grid(char letter, int tile_x, int tile_y)
{
    int x = XMIN + LETTER_WIDTH * tile_x; //letters are bound between XMIN < x < XMIN + 940
    int y = YMAX - LETTER_HEIGHT * (tile_y + 1); //letters are bound between YMAX - 1500 < y < YMAX
    draw_letter(letter, x, y);
}