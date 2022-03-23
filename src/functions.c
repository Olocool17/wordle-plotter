#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <functions.h>
#include <servo.h>

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//functions.c:
//Calculations done assuming L1 (= length of the first servo) = L2 (= length of second servo) = 13.8cm, with the coordinate system also in cm.
//No out of bounds check is needed for find_angles since boundaries were already checked during the bezier call, requiring only 2 or 4 checks instead of 100.
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

//Macro's for x² and x³
#define SQUARE(x) (x)*(x)
#define CUBE(x) (x)*(x)*(x)

void move(double theta_1, double theta_2) { 
    servo1_dutymicros = ((M_PI/2)+theta_1)/M_PI*1600 + 700;
    servo2_dutymicros = theta_2/M_PI*1600 + 700;
}

void move_with_lift(double theta_1, double theta_2) {
    //"lift pen"
    servo1_dutymicros = ((M_PI/2)+theta_1)/M_PI*1600 + 700;
    servo2_dutymicros = theta_2/M_PI*1600 + 700;
    //"lower pen"
}

void find_angles(double x_coor, double y_coor) { 
    double theta_2 = acos((SQUARE(x_coor) + SQUARE(y_coor) - 2 * SQUARE(13.8))/(2 * SQUARE(13.8)) );
    double theta_1 = atan(y_coor/x_coor) + atan(13.8*sin(theta_2)/(13.8 + 13.8*cos(theta_2)));
    move(theta_1, theta_2);
}

void find_angles_with_lift(double x_coor, double y_coor) { 
    double theta_2 = acos((SQUARE(x_coor) + SQUARE(y_coor) - 2 * SQUARE(13.8))/(2 * SQUARE(13.8)) );
    double theta_1 = atan(y_coor/x_coor) + atan(13.8*sin(theta_2)/(13.8 + 13.8*cos(theta_2)));
    move_with_lift(theta_1, theta_2);
}

void lin_bez(double start_x, double start_y, double end_x, double end_y) {
    if (!(start_x >= 0 && start_x <= 15 && start_y >= 0 && start_y <= 21)) {
        assert(0); //OoB check for start
    }
    if (!(end_x >= 0 && end_x <= 15 && end_y >= 0 && end_y <= 21)) {
        assert(0); //OoB check for end
    }
    int t = 0;
    while (t<50) {
        t += 1;
        double x_next = start_x + t*(end_x - start_x)/50;
        double y_next = start_y + t*(end_y - start_y)/50;
        find_angles(x_next, y_next); //transfer next coordinates to the angle finding function
    }
}

void cub_bez(double start_x, double start_y, double cp1_x, double cp1_y, double cp2_x, double cp2_y, double end_x, double end_y) {
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
        double x_next = CUBE(1-scale)*start_x + 3*SQUARE(1-scale)*scale*cp1_x+3*(1-scale)*SQUARE(scale)*cp2_x+CUBE(scale)*end_x;
        double y_next = CUBE(1-scale)*start_y + 3*SQUARE(1-scale)*scale*cp1_y+3*(1-scale)*SQUARE(scale)*cp2_y+CUBE(scale)*end_y;
        find_angles(x_next, y_next); //transfer next coordinates to the angle finding function
    }
}

void draw_A(double x, double y) {
    find_angles_with_lift(1.16 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.16 + 2.8*x, 15.16 - 2.8*y, 1.864 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.864 + 2.8*x, 16.6 - 2.8*y, 2.568 + 2.8*x, 15.16 - 2.8*y);
    find_angles_with_lift(1.512 + 2.8*x, 15.88 - 2.8*y);
    lin_bez(1.512 + 2.8*x, 15.88 - 2.8*y, 2.216 + 2.8*x, 15.88 - 2.8*y);
}
void draw_B(double x, double y) {
    find_angles_with_lift(1.36 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.36 + 2.8*x, 15.16 - 2.8*y, 1.36 + 2.8*x, 16.6 - 2.8*y);
    cub_bez(1.36 + 2.8*x, 16.6 - 2.8*y, 2.2 + 2.8*x, 16.6 - 2.8*y, 2.2 + 2.8*x, 15.88 - 2.8*y, 1.36 + 2.8*x, 15.88 - 2.8*y);
    cub_bez(1.36 + 2.8*x, 15.88 - 2.8*y, 2.2 + 2.8*x, 15.88 - 2.8*y, 2.2 + 2.8*x, 15.16 - 2.8*y, 1.36 + 2.8*x, 15.16 - 2.8*y);
}
void draw_C(double x, double y) {
    find_angles_with_lift(2.4 + 2.8*x, 16.6 - 2.8*y);
    cub_bez(2.4 + 2.8*x, 16.6 - 2.8*y, 0.96 + 2.8*x, 16.6 - 2.8*y, 0.96 + 2.8*x, 15.16 - 2.8*y, 2.4 + 2.8*x, 15.16 - 2.8*y);
}
void draw_D(double x, double y) {
    find_angles_with_lift(0.96 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.16 - 2.8*y, 0.96 + 2.8*x, 16.6 - 2.8*y);
    cub_bez(0.96 + 2.8*x, 16.6 - 2.8*y, 2.4 + 2.8*x, 16.6 - 2.8*y, 2.4 + 2.8*x, 15.16 - 2.8*y, 0.96 + 2.8*x, 15.16 - 2.8*y);
}
void draw_E(double x, double y) {
    find_angles_with_lift(1.68 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.68 + 2.8*x, 15.16 - 2.8*y, 0.96 + 2.8*x, 15.16 - 2.8*y);
    draw_F(x,y);
}
void draw_F(double x, double y) {
    find_angles_with_lift(0.96 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.16 - 2.8*y, 0.96 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 16.6 - 2.8*y, 1.68 + 2.8*x, 16.6 - 2.8*y);
    find_angles_with_lift(0.96 + 2.8*x, 15.88 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.88 - 2.8*y, 1.68 + 2.8*x, 15.88 - 2.8*y);
}
void draw_G(double x, double y) {
    find_angles_with_lift(2.128 + 2.8*x, 16.088 - 2.8*y);
    lin_bez(2.128 + 2.8*x, 16.088 - 2.8*y, 1.888 + 2.8*x, 16.088 - 2.8*y);
    find_angles_with_lift(1.888 + 2.8*x, 16.64 - 2.8*y);
    cub_bez(1.888 + 2.8*x, 16.64 - 2.8*y, 0.8 + 2.8*x, 16.472 - 2.8*y, 1.728 + 2.8*x, 15 - 2.8*y, 2.128 + 2.8*x, 16.088 - 2.8*y);
}
void draw_H(double x, double y) {
    find_angles_with_lift(0.96 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.16 - 2.8*y, 0.96 + 2.8*x, 16.6 - 2.8*y);
    find_angles_with_lift(0.96 + 2.8*x, 15.88 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.88 - 2.8*y, 1.68 + 2.8*x, 15.88 - 2.8*y);
    find_angles_with_lift(1.68 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.68 + 2.8*x, 15.16 - 2.8*y, 1.68 + 2.8*x, 16.6 - 2.8*y);
}
void draw_I(double x, double y) {
    find_angles_with_lift(1.44 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.44 + 2.8*x, 16.6 - 2.8*y, 2.16 + 2.8*x, 16.6 - 2.8*y);
    find_angles_with_lift(1.44 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.44 + 2.8*x, 15.16 - 2.8*y, 2.16 + 2.8*x, 15.16 - 2.8*y);
    find_angles_with_lift(1.8 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.8 + 2.8*x, 15.16 - 2.8*y, 1.8 + 2.8*x, 16.6 - 2.8*y);
}
void draw_J(double x, double y) {
    find_angles_with_lift(1.44 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.44 + 2.8*x, 16.6 - 2.8*y, 2.16 + 2.8*x, 16.6 - 2.8*y);
    find_angles_with_lift(1.8 + 2.8*x, 16.6 - 2.8*y);
    cub_bez(1.8 + 2.8*x, 16.6 - 2.8*y, 1.864 + 2.8*x, 16.168 - 2.8*y, 2.152 + 2.8*x, 15.384 - 2.8*y, 1.44 + 2.8*x, 15.384 - 2.8*y);
}
void draw_K(double x, double y) {
    find_angles_with_lift(0.96 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.16 - 2.8*y, 0.96 + 2.8*x, 16.6 - 2.8*y);
    find_angles_with_lift(1.44 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.44 + 2.8*x, 16.6 - 2.8*y, 0.96 + 2.8*x, 15.88 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.88 - 2.8*y, 1.44 + 2.8*x, 15.16 - 2.8*y);
}
void draw_L(double x, double y) {
    find_angles_with_lift(0.96 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 16.6 - 2.8*y, 0.96 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.16 - 2.8*y, 1.92 + 2.8*x, 15.16 - 2.8*y);
}
void draw_M(double x, double y) {
    find_angles_with_lift(0.96 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.16 - 2.8*y, 0.96 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 16.6 - 2.8*y, 1.52 + 2.8*x, 15.88 - 2.8*y);
    lin_bez(1.52 + 2.8*x, 15.88 - 2.8*y, 2.08 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(2.08 + 2.8*x, 16.6 - 2.8*y, 2.08 + 2.8*x, 15.16 - 2.8*y);
}
void draw_N(double x, double y) {
    find_angles_with_lift(0.96 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 15.16 - 2.8*y, 0.96 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(0.96 + 2.8*x, 16.6 - 2.8*y, 1.92 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.92 + 2.8*x, 15.16 - 2.8*y, 1.92 + 2.8*x, 16.6 - 2.8*y);
}
void draw_O(double x, double y) {
    find_angles_with_lift(1.2 + 2.8*x, 16 - 2.8*y);
    cub_bez(1.2 + 2.8*x, 16 - 2.8*y, 1.2 + 2.8*x, 15.04 - 2.8*y, 2.48 + 2.8*x, 15.04 - 2.8*y, 2.48 + 2.8*x, 16 - 2.8*y);
    cub_bez(2.48 + 2.8*x, 16 - 2.8*y, 2.48 + 2.8*x, 16.96 - 2.8*y, 1.2 + 2.8*x, 16.96 - 2.8*y, 1.2 + 2.8*x, 16 - 2.8*y);
}
void draw_P(double x, double y) {
    find_angles_with_lift(1.36 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.36 + 2.8*x, 15.16 - 2.8*y, 1.36 + 2.8*x, 16.6 - 2.8*y);
    cub_bez(1.36 + 2.8*x, 16.6 - 2.8*y, 2.2 + 2.8*x, 16.6 - 2.8*y, 2.2 + 2.8*x, 15.88 - 2.8*y, 1.36 + 2.8*x, 15.88 - 2.8*y);
}
void draw_Q(double x, double y) {
    find_angles_with_lift(1.88 + 2.8*x, 15.96 - 2.8*y);
    lin_bez(1.88 + 2.8*x, 15.96 - 2.8*y, 2.464 + 2.8*x, 15.24 - 2.8*y);
    find_angles_with_lift(1.2 + 2.8*x, 16 - 2.8*y);
    draw_O(x, y);
}
void draw_R(double x, double y) {
    find_angles_with_lift(1.36 + 2.8*x, 15.16 - 2.8*y);
    lin_bez(1.36 + 2.8*x, 15.16 - 2.8*y, 1.36 + 2.8*x, 16.6 - 2.8*y);
    cub_bez(1.36 + 2.8*x, 16.6 - 2.8*y, 2.2 + 2.8*x, 16.6 - 2.8*y, 2.2 + 2.8*x, 15.88 - 2.8*y, 1.36 + 2.8*x, 15.88 - 2.8*y);
    lin_bez(1.36 + 2.8*x, 15.88 - 2.8*y, 1.92 + 2.8*x, 15.16 - 2.8*y);
}
void draw_S(double x, double y) {
    find_angles_with_lift(2 + 2.8*x, 16.44 - 2.8*y);
    cub_bez(2 + 2.8*x, 16.44 - 2.8*y, 1.04 + 2.8*x, 16.44 - 2.8*y, 2.6 + 2.8*x, 15.456 - 2.8*y, 1.64 + 2.8*x, 15.456 - 2.8*y);
}
void draw_T(double x, double y) {
    find_angles_with_lift(1.12 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.12 + 2.8*x, 16.6 - 2.8*y, 2.56 + 2.8*x, 16.6 - 2.8*y);
    find_angles_with_lift(1.84 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.84 + 2.8*x, 16.6 - 2.8*y, 1.84 + 2.8*x, 15.48 - 2.8*y);
}
void draw_U(double x, double y) {
    find_angles_with_lift(1.4 + 2.8*x, 16.76 - 2.8*y);
    cub_bez(1.4 + 2.8*x, 16.76 - 2.8*y, 1.4 + 2.8*x, 15 - 2.8*y, 2.6 + 2.8*x, 15 - 2.8*y, 2.6 + 2.8*x, 16.76 - 2.8*y);
}
void draw_V(double x, double y) {
    find_angles_with_lift(1.2 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 16.6 - 2.8*y, 1.8 + 2.8*x, 15.48 - 2.8*y);
    lin_bez(1.8 + 2.8*x, 15.48 - 2.8*y, 2.4 + 2.8*x, 16.6 - 2.8*y);
}
void draw_W(double x, double y) {
    find_angles_with_lift(1.04 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.04 + 2.8*x, 16.6 - 2.8*y, 1.4 + 2.8*x, 15.32 - 2.8*y);
    lin_bez(1.4 + 2.8*x, 15.32 - 2.8*y, 1.76 + 2.8*x, 15.96 - 2.8*y);
    lin_bez(1.76 + 2.8*x, 15.96 - 2.8*y, 2.12 + 2.8*x, 15.32 - 2.8*y);
    lin_bez(2.12 + 2.8*x, 15.32 - 2.8*y, 2.48 + 2.8*x, 16.6 - 2.8*y);
}
void draw_X(double x, double y) {
    find_angles_with_lift(1.2 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 16.6 - 2.8*y, 2.48 + 2.8*x, 15.32 - 2.8*y);
    find_angles_with_lift(1.2 + 2.8*x, 15.32 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 15.32 - 2.8*y, 2.48 + 2.8*x, 16.6 - 2.8*y);
}
void draw_Y(double x, double y) {
    find_angles_with_lift(1.2 + 2.8*x, 16.6 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 16.6 - 2.8*y, 1.6 + 2.8*x, 16 - 2.8*y);
    find_angles_with_lift(1.2 + 2.8*x, 15.4 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 15.4 - 2.8*y, 2 + 2.8*x, 16.6 - 2.8*y);
}
void draw_Z(double x, double y) {
    find_angles_with_lift(1.2 + 2.8*x, 16.76 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 16.76 - 2.8*y, 2.4 + 2.8*x, 16.76 - 2.8*y);
    lin_bez(2.4 + 2.8*x, 16.76 - 2.8*y, 1.2 + 2.8*x, 15.4 - 2.8*y);
    lin_bez(1.2 + 2.8*x, 15.4 - 2.8*y, 2.4 + 2.8*x, 15.4 - 2.8*y);
}