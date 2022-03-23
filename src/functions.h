#ifndef FUNCTIONS_LOADED
#define FUNCTIONS_LOADED
//Function to move the servo's when given angles
void move();
//Same function but having the pen lifted between start and end
void move_with_lift();

//Inverse kinematics to find servo-angles
void find_angles();
//Same but calling to move_with_lift at the end
void find_angles_with_lift();

//Function for linear bézier curve
void lin_bez();
//Function for cubic bézier curve
void cub_bez();

//Functions for individual letters
void draw_A();
void draw_B();
void draw_C();
void draw_D();
void draw_E();
void draw_F();
void draw_G();
void draw_H();
void draw_I();
void draw_J();
void draw_K();
void draw_L();
void draw_M();
void draw_N();
void draw_O();
void draw_P();
void draw_Q();
void draw_R();
void draw_S();
void draw_T();
void draw_U();
void draw_V();
void draw_W();
void draw_X();
void draw_Y();
void draw_Z();
#endif