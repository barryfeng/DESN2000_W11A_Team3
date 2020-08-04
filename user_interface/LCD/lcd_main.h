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
#define MAX_STOPS 8
#define MAX_STOP_STRING_SIZE 12

void init_lcd(void);
void lcd_run(void);

void constant_borders(void);
void constant_labels(void);

void lcd_backlight_on(void);
void lcd_backlight_off(void);

void constant_velocity_on(void);
void constant_throttle_plus_on(void);
void constant_throttle_minus_on(void);
void constant_stops_on(void);
void constant_brake_on(void);
void constant_dms_on(void);
void constant_max_vel_on(void);
void constant_sd_on(void);

void constant_velocity_off(void);
void constant_throttle_plus_off(void);
void constant_throttle_minus_off(void);
void constant_stops_off(void);
void constant_brake_off(void);
void constant_dms_off(void);
void constant_max_vel_off(void);
void constant_sd_off(void);

void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, int thickness, lcd_color_t color);
void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, int thickness, lcd_color_t color);

#endif
