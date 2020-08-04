#ifndef TCHSCRNINIT_H

#define TCHSCRNINIT_H

#include "button_press.h"
#include "touch.h"
#include "string.h"
#include "stdio.h"

void touch_screen_press(void);
void pressure_calc(char *pressure, char x, char y, char z1, char z2);

#endif
