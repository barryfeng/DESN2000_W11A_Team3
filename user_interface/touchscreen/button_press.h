#ifndef BUTNPRESS_H

#include "master_controller.h"

#define BUTNPRESS_H

//Location and size of buttons on screen
#define THROTTLE_PLUS_X0 200
#define THROTTLE_PLUS_X1 234
#define THROTTLE_PLUS_Y0 68
#define THROTTLE_PLUS_Y1 149
#define THROTTLE_MINUS_X0 200
#define THROTTLE_MINUS_X1 234
#define THROTTLE_MINUS_Y0 154
#define THROTTLE_MINUS_Y1 235
#define SD_X0 7
#define SD_X1 39
#define SD_Y0 264
#define SD_Y1 314
#define BRAKE_X 200
#define BRAKE_Y 280
#define BRAKE_R 35

void press_button(char x, char y);
// void blow_bubbles(void);
// void newbubble(void);
int inside_brake(int point_x, int point_y);
int inside_throttle_plus(int point_x, int point_y);
int inside_throttle_minus(int point_x, int point_y);
int inside_sd(int point_x, int point_y);

#endif