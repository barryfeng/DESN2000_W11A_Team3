#ifndef LCDMAIN_H

#define LCDMAIN_H

#include "lpc24xx.h"
#include "delay.h"
#include "lcd/lcd_hw.h"
#include "lcd/lcd_grph.h"
#include "lcd/lcd_cfg.h"
#include "lcd/sdram.h"
#include <master_controller.h>
#include <stdlib.h>

#define OFF 0
#define ON 1
#define RELEASE 0
#define APPLY 1
#define STOP 0
#define MAX_VEL 50

void lcd_start();
void lcd_run();

void constant_borders();
void constant_labels();

void constant_velocity_on();
void constant_throttle_plus_on();
void constant_throttle_minus_on();
void constant_stops_on();
void constant_brake_on();
void constant_dms_on();
void constant_max_vel_on();
void constant_sd_on();

void constant_velocity_off();
void constant_throttle_plus_off();
void constant_throttle_minus_off();
void constant_stops_off();
void constant_brake_off();
void constant_dms_off();
void constant_max_vel_off();
void constant_sd_off();

void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, int thickness, lcd_color_t color);
void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, int thickness, lcd_color_t color);


#endif