#include <stdbool.h>
#include <math.h>
//Assuming L1 (= length of the first servo) = L2 (= length of second servo) = 13.8cm, where the coordinate system is also in cm.
void move(double theta_1, double theta_2) { //Moving the servo's given the angles
    return;
}

void find_angles(double x_coor, double y_coor) { //finding the angles corresponding to X- and Y-coordinates.
    double theta_2 = acos((x_coor * x_coor + y_coor * y_coor - 2 * 13.8 * 13.8)/(2 * 13.8 * 13.8) );
    double theta_1 = atan(y_coor/x_coor) + atan(13.8*sin(theta_2)/(13.8 + 13.8*cos(theta_2)));
    move(theta_1, theta_2);
    return;
}

void lin_bez(double start_x_coor, double start_y_coor, double end_x_coor, double end_y_coor) { //Function for Linear bezier curve
//search for possibility of calling the waypoints with 1 variable each?
//Implement a special function to move pen to the starting point after lifting the pen
    int t = 0;
    while (t<50) {
        t += 1;
        double x_next = start_x_coor + t*(end_x_coor - start_x_coor)/50;
        double y_next = start_y_coor + t*(end_y_coor - start_y_coor)/50;
        find_angles(x_next, y_next); //transfer next coordinates to the angle finding function
    }
    return (0);
}

void cub_bez(double start_x_coor, double start_y_coor, double cp1_x_coor, double cp1_y_coor, double cp2_x_coor, double cp2_y_coor, double end_x_coor, double end_y_coor) {
    // Function for Cubic bezier curve
    int t = 0;
    while (t<50) {
        t += 1;
        double scale = t/50;
        double x_next = (1-scale)*(1-scale)*(1-scale)*start_x_coor + 3*(1-scale)*(1-scale)*scale*cp1_x_coor+3*(1-scale)*scale*scale*cp2_x_coor+scale*scale*scale*end_x_coor;
        double y_next = (1-scale)*(1-scale)*(1-scale)*start_y_coor + 3*(1-scale)*(1-scale)*scale*cp1_y_coor+3*(1-scale)*scale*scale*cp2_y_coor+scale*scale*scale*end_y_coor;
        find_angles(x_next, y_next); //transfer next coordinates to the angle finding function
    }
    return (0);
}

//x = 0.8 + 2.8*x_offset + 2*x_value/250
//y = 15 - 2.8*y_offset + 2*y_value/250
//all values given in the comments are on a scale of 250
//IMPORTANT: test all letters before demonstration for errors, and test them all on the second guess or after and as the second letter or after so that x_offset or y_offset are not equal to 0.
void draw_A(double x_offset, double y_offset) {
    lin_bez(1.16 + 2.8*x_offset, 15.16 - 2.8*y_offset, 1.864 + 2.8*x_offset, 16.6 - 2.8 * y_offset);
    lin_bez(1.864 + 2.8*x_offset, 16.6 - 2.8 * y_offset, 2.568 + 2.8*x_offset, 15.16 - 2.8*y_offset);
    lin_bez(1.512 + 2.8*x_offset, 15.88 - 2.8* y_offset, 2.216 + 2.8 * x_offset, 15.88 - 2.8 * y_offset);
    return;
}

void draw_B(double x_offset, double y_offset) {
    lin_bez(1.36 + 2.8*x_offset, 15.16 - 2.8*y_offset, 1.36 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    cub_bez(1.36 + 2.8*x_offset, 16.6 - 2.8*y_offset, 2.2 + 2.8*x_offset, 16.6 - 2.8*y_offset, 2.2 + 2.8*x_offset, 15.88 - 2.8*y_offset, 1.36 + 2.8*x_offset, 15.88 - 2.8*y_offset);
    cub_bez(1.36 + 2.8*x_offset, 15.88 - 2.8*y_offset, 2.2 + 2.8*x_offset, 15.88 - 2.8*y_offset, 2.2 + 2.8*x_offset, 15.16 - 2.8*y_offset, 1.36 + 2.8*x_offset, 15.16 - 2.8*y_offset);
    return;
}

void draw_C(double x_offset, double y_offset) {
    cub_bez(2.4 + 2.8*x_offset, 16.6 - 2.8*y_offset, 0.96 + 2.8*x_offset, 16.6 - 2.8*y_offset, 0.96 + 2.8*x_offset, 15.16 - 2.8*y_offset, 2.4 + 2.8*x_offset, 15.16 - 2.8*y_offset);
    return;
}

void draw_D(double x_offset, double y_offset) {
    lin_bez(0.96 + 2.8 * x_offset, 15.16 - 2.8*y_offset, 0.96 + 2.8*x_offset, 16.6 - 2.8 * y_offset);
    cub_bez(0.96 + 2.8 * x_offset, 16.6 - 2.8 * y_offset, 2.4 + 2.8*x_offset, 16.6 - 2.8*y_offset, 2.4 + 2.8*x_offset, 15.16 - 2.8 * y_offset, 0.96 + 2.8 * x_offset, 15.16 - 2.8*y_offset);
    return;
}

void draw_E(double x_offset, double y_offset) {
    lin_bez(1.68 + 2.8*x_offset, 15.16 - 2.8*y_offset, 0.96 + 2.8*x_offset, 15.16 - 2.8*y_offset);
    draw_F(x_offset,y_offset); //dab B)
    return;
}

void draw_F(double x_offset, double y_offset) {
    lin_bez(0.96 + 2.8*x_offset, 15.16 - 2.8*y_offset, 0.96 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    lin_bez(0.96 + 2.8*x_offset, 16.6 - 2.8*y_offset, 1.68 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    lin_bez(0.96 + 2.8*x_offset, 15.88 - 2.8*y_offset, 1.68 + 2.8*x_offset, 15.88 - 2.8*y_offset);
    return;
}

void draw_G(double x_offset, double y_offset) {
    lin_bez(2.128 + 2.8*x_offset, 16.088 - 2.8*y_offset, 1.888 + 2.8*x_offset, 16.088 - 2.8*y_offset);
    cub_bez(1.888 + 2.8*x_offset, 16.64 - 2.8*y_offset, 0.8 + 2.8*x_offset, 16.472 - 2.8*y_offset, 1.728 + 2.8*x_offset, 15 - 2.8*y_offset, 2.128 + 2.8*x_offset, 16.088 - 2.8*y_offset);
    return;
}

void draw_H(double x_offset, double y_offset) {
    lin_bez(0.96 + 2.8*x_offset, 15.16 - 2.8*y_offset, 0.96 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    lin_bez(0.96 + 2.8*x_offset, 15.88 - 2.8*y_offset, 1.68 + 2.8*x_offset, 15.88 - 2.8*y_offset);
    lin_bez(1.68 + 2.8*x_offset, 15.16 - 2.8*y_offset, 1.68 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    return;
}

void draw_I(double x_offset, double y_offset) {
    lin_bez(1.44 + 2.8*x_offset, 16.6 - 2.8*y_offset, 2.16 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    lin_bez(1.44 + 2.8*x_offset, 15.16 - 2.8*y_offset, 2.16 + 2.8*x_offset, 15.16 - 2.8*y_offset);
    lin_bez(1.8 + 2.8*x_offset, 15.16 - 2.8*y_offset, 1.8 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    return;
}

void draw_J(double x_offset, double y_offset) {
    lin_bez(1.44 + 2.8 * x_offset, 16.6 - 2.8 * y_offset, 2.16 + 2.8 * x_offset, 16.6 - 2.8 * y_offset);
    cub_bez(1.8 + 2.8 * x_offset, 16.6 - 2.8 * y_offset, 1.864 + 2.8 * x_offset, 16.168 - 2.8 * y_offset, 2.152 + 2.8 * x_offset, 15.384 - 2.8 * y_offset, 1.44 + 2.8 * x_offset, 15.384 - 2.8 * y_offset);
    return;
}

void draw_K(double x_offset, double y_offset) {
    lin_bez(0.96 + 2.8*x_offset, 15.16 - 2.8*y_offset, 0.96 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    lin_bez(0.96 + 2.8*x_offset, 15.88 - 2.8*y_offset, 1.44 + 2.8*x_offset, 16.6 - 2.8*y_offset);
    lin_bez(0.96 + 2.8*x_offset, 15.88 - 2.8*y_offset, 1.44 + 2.8*x_offset, 15.16 - 2.8*y_offset);
    return;
}

void draw_L(double x_offset, double y_offset) {
    lin_bez(0.96 + 2.8*x_offset, 16.6 - 2.8*y_offset, 0.96 + 2.8*x_offset, 15.16 - 2.8*y_offset);
    lin_bez(0.96 + 2.8*x_offset, 15.16 - 2.8*y_offset, 1.92 + 2.8*x_offset, 15.16 - 2.8*y_offset);
    return;
}

void draw_M(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    lin_bez();
    lin_bez();
    return;
}

void draw_N(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    lin_bez();
    return;
}

void draw_O(double x_offset, double y_offset) {
    cub_bez(1.2 + 2.8*x_offset, 16 - 2.8*y_offset, 1.2 + 2.8 * x_offset, 15.04 - 2.8*y_offset, 2.48 + 2.8*x_offset, 15.04 - 2.8*y_offset, 2.48 + 2.8*x_offset, 16 - 2.8*y_offset);
    cub_bez(2.48 + 2.8*x_offset, 16 - 2.8*y_offset, 2.48 + 2.8 * x_offset, 16.96 - 2.8*y_offset, 1.2 + 2.8*x_offset, 16.96 - 2.8*y_offset, 1.2 + 2.8*x_offset, 16 - 2.8*y_offset);
    return;
}

void draw_P(double x_offset, double y_offset) {
    lin_bez();
    cub_bez();
    return;
}

void draw_Q(double x_offset, double y_offset) {
    lin_bez(1.88 + 2.8*x_offset, 15.96 - 2.8*y_offset, 2.464 + 2.8*x_offset, 15.24 - 2.8*y_offset);
    draw_O(x_offset, y_offset); //dab B)
    return;
}

void draw_R(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    cub_bez();
    return;
}

void draw_S(double x_offset, double y_offset) {
    cub_bez();
    return;
}

void draw_T(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    return;
}

void draw_U(double x_offset, double y_offset) {
    cub_bez(1.4 + 2.8 * x_offset, 16.76 - 2.8 * y_offset, 1.4 + 2.8*x_offset, 15 - 2.8*y_offset, 2.6 + 2.8*x_offset, 15 - 2.8*y_offset, 2.6 + 2.8*x_offset, 16.76 - 2.8*y_offset);
    return;
}

void draw_V(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    return;
}

void draw_W(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    lin_bez();
    lin_bez();
    return;
}

void draw_X(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    return;
}

void draw_Y(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    return;
}

void draw_Z(double x_offset, double y_offset) {
    lin_bez();
    lin_bez();
    lin_bez();
    return;
}
//kill me (X_X)