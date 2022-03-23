#ifndef FUNCTIONS_LOADED
#define FUNCTIONS_LOADED
//Function to move the servo's when given angles
void move(double theta_1, double theta_2);
//Same function but having the pen lifted between start and end
void move_with_lift(double theta_1, double theta_2);

//Inverse kinematics to find servo-angles
void find_angles(double x_coor, double y_coor);
//Same but calling to move_with_lift at the end
void find_angles_with_lift(double x_coor, double y_coor);

//Function for linear bézier curve
void lin_bez(double start_x, double start_y, double end_x, double end_y);
//Function for cubic bézier curve
void cub_bez(double start_x, double start_y, double cp1_x, double cp1_y, double cp2_x, double cp2_y, double end_x, double end_y);

//Functions for individual letters
void draw_A(float x, float y);
void draw_B(float x, float y);
void draw_C(float x, float y);
void draw_D(float x, float y);
void draw_E(float x, float y);
void draw_F(float x, float y);
void draw_G(float x, float y);
void draw_H(float x, float y);
void draw_I(float x, float y);
void draw_J(float x, float y);
void draw_K(float x, float y);
void draw_L(float x, float y);
void draw_M(float x, float y);
void draw_N(float x, float y);
void draw_O(float x, float y);
void draw_P(float x, float y);
void draw_Q(float x, float y);
void draw_R(float x, float y);
void draw_S(float x, float y);
void draw_T(float x, float y);
void draw_U(float x, float y);
void draw_V(float x, float y);
void draw_W(float x, float y);
void draw_X(float x, float y);
void draw_Y(float x, float y);
void draw_Z(float x, float y);

//Functions for indicating the correctness of the user guesses
void draw_black(float x, float y);
void draw_yellow(float x, float y);
void draw_green(float x, float y);
#endif