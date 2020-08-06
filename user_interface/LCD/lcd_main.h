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
#define FALSE 0
#define TRUE 1

// LCD Startup
void init_lcd(void);

// LCD Run
void lcd_run(void);

// LCD Startup Statics
void lcd_borders(void);
void lcd_labels(void);

// LCD Mode
void lcd_active_backlight_on(void);
void lcd_standby_backlight_off(void);

// LCD Run Statics (ON)
void lcd_velocity_on(void);
void lcd_throttle_plus_on(void);
void lcd_throttle_minus_on(void);
void lcd_stops_on(void);
void lcd_brake_on(void);
void lcd_dms_on(void);
void lcd_max_vel_on(void);
void lcd_sd_on(void);

// LCD Run Statics (OFF)
void lcd_velocity_off(void);
void lcd_throttle_plus_off(void);
void lcd_throttle_minus_off(void);
void lcd_stops_off(void);
void lcd_brake_off(void);
void lcd_dms_off(void);
void lcd_max_vel_off(void);

// LCD Shape Thickness
void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, int thickness, lcd_color_t color);
void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, int thickness, lcd_color_t color);

// Integer To String Converter
void reverse_string(unsigned char string[], int length);
unsigned char* integer_to_string(int integer, unsigned char* string, int radix);

#endif
