#include <stdbool.h>
#include <util/delay.h>
#include <math.h>
#include <assert.h>
#include <functions.h>
#include <servo.h>

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//functions.c:
//Calculations done assuming L1 (= length of the first servo) = L2 (= length of second servo) = 13.8cm, with the coordinate system also in cm.
//No out of bounds check is needed for move_xy since boundaries were already checked during the bezier call, requiring only 2 or 4 checks instead of 100.
//The boundaries are those of an A5-sheet, being 15x21cm (rounded up)
//For the individual letters, the following formulas apply:
//value of x-position = (0.8 + 2*x_value/250) + 2.8*x
//value of y-position = (15 + 2*y_value/250) - 2.8*y
//the variables x and y are the discrete coordinates of the tiles on the 5x6 wordle grid, with (0,0) being the tile in the TOP left corner!
//the variables x_value and y_value are the x and y-values within an individual gridspace, both on a scale of 250 (each gridspace is 2cmx2cm in size)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//TODO:
//1) Test every letter in a position that checks all variables.
//2) Add pen-lifting function with corresponding dutycycle.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

//Bounds of the board

#define XMIN 0
#define YMIN 3
#define XMAX 14
#define YMAX 21

//Iterations per cm

#define ITERS 3
#define ITER_DELAY 60

//Macro's for x² and x³
#define SQUARE(x) (x)*(x)
#define CUBE(x) (x)*(x)*(x)

void move(double theta_1, double theta_2) { 
    servo1_dutymicros = theta_1/M_PI*1800 + 700;
    servo2_dutymicros = theta_2/M_PI*1780 + 570;
}

void move_with_lift(double theta_1, double theta_2) {
    _delay_ms(150);
    //"lift pen"
    _delay_ms(150);
    servo1_dutymicros = theta_1/M_PI*1800 + 700;
    servo2_dutymicros = theta_2/M_PI*1780 + 570;
    _delay_ms(150);
    //"lower pen"
    _delay_ms(150);
}

void move_xy(double x_coor, double y_coor) { 
    double r = sqrt(SQUARE(x_coor) + SQUARE(y_coor));
    double theta_2 = M_PI - 2*asin(r / (13.8*2));
    double theta_1 = M_PI - asin(y_coor / r) - acos(r / (13.8 * 2));
    move(theta_1, theta_2);
}

void move_xy_with_lift(double x_coor, double y_coor) { 
    double r = sqrt(SQUARE(x_coor) + SQUARE(y_coor));
    double theta_2 = M_PI - 2*asin(r / (13.8*2));
    double theta_1 = M_PI - asin(y_coor / r) - acos(r / (13.8 * 2));
    move_with_lift(theta_1, theta_2);
}

bool within_bounds(double x_coor, double y_coor)
{
    if ((x_coor <  XMIN)|| (x_coor > XMAX) || (y_coor < YMIN) || (y_coor > YMAX))
    {
        return false;
    }
    return true;
}

void lin_bez(double start_x, double start_y, double end_x, double end_y) 
{
    if (!(within_bounds(start_x,start_y) && within_bounds(end_x,end_y))) return;
    float pathlength = sqrt(SQUARE(end_x - start_x) + SQUARE(end_y - start_y));
    int segments = ITERS * pathlength;
    for(size_t t = 0; t <= segments ; t++) 
    {
        double x_next = start_x + t*(end_x - start_x)/segments;
        double y_next = start_y + t*(end_y - start_y)/segments;
        _delay_ms(ITER_DELAY);
        move_xy(x_next, y_next); //transfer next coordinates to the angle finding function
    }
}

void cub_bez(double start_x, double start_y, double cp1_x, double cp1_y, double cp2_x, double cp2_y, double end_x, double end_y) {
    _delay_ms(25);
    if (!(start_x >= 0 && start_x <= 15 && start_y >= 0 && start_y <= 21)) {
        assert(0); //OoB check for start
    }
    if (!(cp1_x >= 0 && cp1_x <= 15 && cp1_y >= 0 && cp1_y <= 21)) {
        assert(0); //OoB check for first control point
    }
    if (!(cp2_x >= 0 && cp2_x <= 15 && cp2_y >= 0 && cp2_y <= 21)) {
        assert(0); //OoB check for second control point
    }
    if (!(end_x >= 0 && end_x <= 15 && end_y >= 0 && end_y <= 21)) {
        assert(0); //OoB check for end
    }
    int t = 0;
    while (t<50) {
        t += 1;
        double scale = t/50;
        double x_next = CUBE(1-scale)*start_x + 3*SQUARE(1-scale)*scale*cp1_x + 3*(1-scale)*SQUARE(scale)*cp2_x + CUBE(scale)*end_x;
        double y_next = CUBE(1-scale)*start_y + 3*SQUARE(1-scale)*scale*cp1_y + 3*(1-scale)*SQUARE(scale)*cp2_y + CUBE(scale)*end_y;
        _delay_ms(15);
        move_xy(x_next, y_next); //transfer next coordinates to the angle finding function
    }
}

void draw_grid()
{
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
void draw_A(float x, float y) {
    move_xy_with_lift(1.16 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.16 + 2.8*x, 18.16 - 2.8*y, 1.864 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.864 + 2.8*x, 19.6 - 2.8*y, 2.568 + 2.8*x, 18.16 - 2.8*y);
    move_xy_with_lift(1.512 + 2.8*x, 18.88 - 2.8*y);
    lin_bez(1.512 + 2.8*x, 18.88 - 2.8*y, 2.216 + 2.8*x, 18.88 - 2.8*y);
}
void draw_B(float x, float y) {
    move_xy_with_lift(1.36 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.36 + 2.8*x, 18.16 - 2.8*y, 1.36 + 2.8*x, 19.6 - 2.8*y);
    cub_bez(1.36 + 2.8*x, 19.6 - 2.8*y, 2.2 + 2.8*x, 19.6 - 2.8*y, 2.2 + 2.8*x, 18.88 - 2.8*y, 1.36 + 2.8*x, 18.88 - 2.8*y);
    cub_bez(1.36 + 2.8*x, 18.88 - 2.8*y, 2.2 + 2.8*x, 18.88 - 2.8*y, 2.2 + 2.8*x, 18.16 - 2.8*y, 1.36 + 2.8*x, 18.16 - 2.8*y);
}
void draw_C(float x, float y) {
    move_xy_with_lift(2.4 + 2.8*x, 19.6 - 2.8*y);
    cub_bez(2.4 + 2.8*x, 19.6 - 2.8*y, 0.96 + 2.8*x, 19.6 - 2.8*y, 0.96 + 2.8*x, 18.16 - 2.8*y, 2.4 + 2.8*x, 18.16 - 2.8*y);
}
void draw_D(float x, float y) {
    move_xy_with_lift(0.96 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.16 - 2.8*y, 0.96 + 2.8*x, 19.6 - 2.8*y);
    cub_bez(0.96 + 2.8*x, 19.6 - 2.8*y, 2.4 + 2.8*x, 19.6 - 2.8*y, 2.4 + 2.8*x, 18.16 - 2.8*y, 0.96 + 2.8*x, 18.16 - 2.8*y);
}
void draw_E(float x, float y) {
    move_xy_with_lift(1.68 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.68 + 2.8*x, 18.16 - 2.8*y, 0.96 + 2.8*x, 18.16 - 2.8*y);
    draw_F(x,y);
}
void draw_F(float x, float y) {
    move_xy_with_lift(0.96 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.16 - 2.8*y, 0.96 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 19.6 - 2.8*y, 1.68 + 2.8*x, 19.6 - 2.8*y);
    move_xy_with_lift(0.96 + 2.8*x, 18.88 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.88 - 2.8*y, 1.68 + 2.8*x, 18.88 - 2.8*y);
}
void draw_G(float x, float y) {
    move_xy_with_lift(2.128 + 2.8*x, 19.088 - 2.8*y);
    lin_bez(2.128 + 2.8*x, 19.088 - 2.8*y, 1.888 + 2.8*x, 19.088 - 2.8*y);
    move_xy_with_lift(1.888 + 2.8*x, 19.64 - 2.8*y);
    cub_bez(1.888 + 2.8*x, 19.64 - 2.8*y, 0.8 + 2.8*x, 19.472 - 2.8*y, 1.728 + 2.8*x, 18 - 2.8*y, 2.128 + 2.8*x, 19.088 - 2.8*y);
}
void draw_H(float x, float y) {
    move_xy_with_lift(0.96 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.16 - 2.8*y, 0.96 + 2.8*x, 19.6 - 2.8*y);
    move_xy_with_lift(0.96 + 2.8*x, 18.88 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.88 - 2.8*y, 1.68 + 2.8*x, 18.88 - 2.8*y);
    move_xy_with_lift(1.68 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.68 + 2.8*x, 18.16 - 2.8*y, 1.68 + 2.8*x, 19.6 - 2.8*y);
}
void draw_I(float x, float y) {
    move_xy_with_lift(1.44 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.44 + 2.8*x, 19.6 - 2.8*y, 2.16 + 2.8*x, 19.6 - 2.8*y);
    move_xy_with_lift(1.44 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.44 + 2.8*x, 18.16 - 2.8*y, 2.16 + 2.8*x, 18.16 - 2.8*y);
    move_xy_with_lift(1.8 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.8 + 2.8*x, 18.16 - 2.8*y, 1.8 + 2.8*x, 19.6 - 2.8*y);
}
void draw_J(float x, float y) {
    move_xy_with_lift(1.44 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.44 + 2.8*x, 19.6 - 2.8*y, 2.16 + 2.8*x, 19.6 - 2.8*y);
    move_xy_with_lift(1.8 + 2.8*x, 19.6 - 2.8*y);
    cub_bez(1.8 + 2.8*x, 19.6 - 2.8*y, 1.864 + 2.8*x, 19.168 - 2.8*y, 2.152 + 2.8*x, 18.384 - 2.8*y, 1.44 + 2.8*x, 18.384 - 2.8*y);
}
void draw_K(float x, float y) {
    move_xy_with_lift(0.96 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.16 - 2.8*y, 0.96 + 2.8*x, 19.6 - 2.8*y);
    move_xy_with_lift(1.44 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.44 + 2.8*x, 19.6 - 2.8*y, 0.96 + 2.8*x, 18.88 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.88 - 2.8*y, 1.44 + 2.8*x, 18.16 - 2.8*y);
}
void draw_L(float x, float y) {
    move_xy_with_lift(0.96 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 19.6 - 2.8*y, 0.96 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.16 - 2.8*y, 1.92 + 2.8*x, 18.16 - 2.8*y);
}
void draw_M(float x, float y) {
    move_xy_with_lift(0.96 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.16 - 2.8*y, 0.96 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 19.6 - 2.8*y, 1.52 + 2.8*x, 18.88 - 2.8*y);
    lin_bez(1.52 + 2.8*x, 18.88 - 2.8*y, 2.08 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(2.08 + 2.8*x, 19.6 - 2.8*y, 2.08 + 2.8*x, 18.16 - 2.8*y);
}
void draw_N(float x, float y) {
    move_xy_with_lift(0.96 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 18.16 - 2.8*y, 0.96 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 19.6 - 2.8*y, 1.92 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.92 + 2.8*x, 18.16 - 2.8*y, 1.92 + 2.8*x, 19.6 - 2.8*y);
}
void draw_O(float x, float y) {
    move_xy_with_lift(1.2 + 2.8*x, 19 - 2.8*y);
    cub_bez(1.2 + 2.8*x, 19 - 2.8*y, 1.2 + 2.8*x, 18.04 - 2.8*y, 2.48 + 2.8*x, 18.04 - 2.8*y, 2.48 + 2.8*x, 19 - 2.8*y);
    cub_bez(2.48 + 2.8*x, 19 - 2.8*y, 2.48 + 2.8*x, 19.96 - 2.8*y, 1.2 + 2.8*x, 19.96 - 2.8*y, 1.2 + 2.8*x, 19 - 2.8*y);
}
void draw_P(float x, float y) {
    move_xy_with_lift(1.36 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.36 + 2.8*x, 18.16 - 2.8*y, 1.36 + 2.8*x, 19.6 - 2.8*y);
    cub_bez(1.36 + 2.8*x, 19.6 - 2.8*y, 2.2 + 2.8*x, 19.6 - 2.8*y, 2.2 + 2.8*x, 18.88 - 2.8*y, 1.36 + 2.8*x, 18.88 - 2.8*y);
}
void draw_Q(float x, float y) {
    move_xy_with_lift(1.88 + 2.8*x, 18.96 - 2.8*y);
    lin_bez(1.88 + 2.8*x, 18.96 - 2.8*y, 2.464 + 2.8*x, 18.24 - 2.8*y);
    draw_O(x, y);
}
void draw_R(float x, float y) {
    move_xy_with_lift(1.36 + 2.8*x, 18.16 - 2.8*y);
    lin_bez(1.36 + 2.8*x, 18.16 - 2.8*y, 1.36 + 2.8*x, 19.6 - 2.8*y);
    cub_bez(1.36 + 2.8*x, 19.6 - 2.8*y, 2.2 + 2.8*x, 19.6 - 2.8*y, 2.2 + 2.8*x, 18.88 - 2.8*y, 1.36 + 2.8*x, 18.88 - 2.8*y);
    lin_bez(1.36 + 2.8*x, 18.88 - 2.8*y, 1.92 + 2.8*x, 18.16 - 2.8*y);
}
void draw_S(float x, float y) {
    move_xy_with_lift(2 + 2.8*x, 19.44 - 2.8*y);
    cub_bez(2 + 2.8*x, 19.44 - 2.8*y, 1.04 + 2.8*x, 19.44 - 2.8*y, 2.6 + 2.8*x, 18.456 - 2.8*y, 1.64 + 2.8*x, 18.456 - 2.8*y);
}
void draw_T(float x, float y) {
    move_xy_with_lift(1.12 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.12 + 2.8*x, 19.6 - 2.8*y, 2.56 + 2.8*x, 19.6 - 2.8*y);
    move_xy_with_lift(1.84 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.84 + 2.8*x, 19.6 - 2.8*y, 1.84 + 2.8*x, 18.48 - 2.8*y);
}
void draw_U(float x, float y) {
    move_xy_with_lift(1.4 + 2.8*x, 19.76 - 2.8*y);
    cub_bez(1.4 + 2.8*x, 19.76 - 2.8*y, 1.4 + 2.8*x, 18 - 2.8*y, 2.6 + 2.8*x, 18 - 2.8*y, 2.6 + 2.8*x, 19.76 - 2.8*y);
}
void draw_V(float x, float y) {
    move_xy_with_lift(1.2 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 19.6 - 2.8*y, 1.8 + 2.8*x, 18.48 - 2.8*y);
    lin_bez(1.8 + 2.8*x, 18.48 - 2.8*y, 2.4 + 2.8*x, 19.6 - 2.8*y);
}
void draw_W(float x, float y) {
    move_xy_with_lift(1.04 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.04 + 2.8*x, 19.6 - 2.8*y, 1.4 + 2.8*x, 18.32 - 2.8*y);
    lin_bez(1.4 + 2.8*x, 18.32 - 2.8*y, 1.76 + 2.8*x, 18.96 - 2.8*y);
    lin_bez(1.76 + 2.8*x, 18.96 - 2.8*y, 2.12 + 2.8*x, 18.32 - 2.8*y);
    lin_bez(2.12 + 2.8*x, 18.32 - 2.8*y, 2.48 + 2.8*x, 19.6 - 2.8*y);
}
void draw_X(float x, float y) {
    move_xy_with_lift(1.2 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 19.6 - 2.8*y, 2.48 + 2.8*x, 18.32 - 2.8*y);
    move_xy_with_lift(1.2 + 2.8*x, 18.32 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 18.32 - 2.8*y, 2.48 + 2.8*x, 19.6 - 2.8*y);
}
void draw_Y(float x, float y) {
    move_xy_with_lift(1.2 + 2.8*x, 19.6 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 19.6 - 2.8*y, 1.6 + 2.8*x, 19 - 2.8*y);
    move_xy_with_lift(1.2 + 2.8*x, 18.4 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 18.4 - 2.8*y, 2 + 2.8*x, 19.6 - 2.8*y);
}
void draw_Z(float x, float y) {
    move_xy_with_lift(1.2 + 2.8*x, 19.76 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 19.76 - 2.8*y, 2.4 + 2.8*x, 19.76 - 2.8*y);
    lin_bez(2.4 + 2.8*x, 19.76 - 2.8*y, 1.2 + 2.8*x, 18.4 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 18.4 - 2.8*y, 2.4 + 2.8*x, 18.4 - 2.8*y);
}

void draw_black(float x, float y) {
    move_xy_with_lift(1.68 + 2.8*x, 20.48 - 2.8*y);
    lin_bez(1.68 + 2.8*x, 20.48 - 2.8*y, 2.0 + 2.8*x, 20.16 - 2.8*y);
    move_xy_with_lift(1.68 + 2.8*x, 20.16 - 2.8*y);
    lin_bez(1.68 + 2.8*x, 20.16 - 2.8*y, 2.0 + 2.8*x, 20.48 - 2.8*y);
}
void draw_yellow(float x, float y) {
    move_xy_with_lift(1.504 + 2.8*x, 20.28 - 2.8*y);
    cub_bez(1.504 + 2.8*x, 20.28 - 2.8*y, 1.776 + 2.8*x, 20.64 - 2.8*y, 1.776 + 2.8*x, 19.92 - 2.8*y, 2.088 + 2.8*x, 20.376 - 2.8*y);
}
void draw_green(float x, float y) {
    move_xy_with_lift(1.68 + 2.8*x, 20.48 - 2.8*y);
    lin_bez(1.68 + 2.8*x, 20.48 - 2.8*y, 1.84 + 2.8*x, 20.16 - 2.8*y);
    lin_bez(1.84 + 2.8*x, 20.16 - 2.8*y, 2.0 + 2.8*x, 20.48 - 2.8*y);
}