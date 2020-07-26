#include "lpc24xx.h"
#include "delay.h"
#include "lcd/lcd_hw.h"
#include "lcd/lcd_grph.h"
#include "lcd/lcd_cfg.h"
#include "lcd/sdram.h"

// EXTRA #INCLUDES
#include <master_controller.h>

/*
void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x0, unsigned short y0, int thickness, lcd_color_t color);
void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, int thickness, lcd_color_t color);
void lcd_throttle_lever(int throttle_position);
void lcd_indicator_state(unsigned short x0, unsigned short y0, unsigned short r, int state);
*/

int main(void) {
	//Setup external SDRAM. Used for Frame Buffer
	sdramInit();
	
	//Setup LPC2478 LCD Controller for our specific LCD
	//lcd_config is defined in lcd/lcd_cfg.h
	lcdInit(&lcd_config);
	
	//Turn the LCD on
	lcdTurnOn();
	
	//Draw a background with lcd_fillScreen
	//Otherwise you will write random noise to the screen!
	lcd_fillScreen(CUSTOM_1); //

	/********** STATIC *********/

	//BORDERS

	//Outer Border
	lcd_drawRect(1, 1, 240, 320, CUSTOM_2); //
	//Current Stop Border
	lcd_drawRect(2, 2, 120, 47, CUSTOM_2); //
	//Next Stop Border
	lcd_drawRect(121, 2, 239, 47, CUSTOM_2); //
	//Velocity Border
	lcd_drawRect(2, 48, 194, 240, CUSTOM_2); //
	//Throttle Border
	lcd_drawRect(195, 48, 239, 240, CUSTOM_2); //
	//Safety Indicators Border
	lcd_drawRect(2, 241, 160, 319, CUSTOM_2); //
	//Brake Border
	lcd_drawRect(161, 241, 239, 319, CUSTOM_2); //

	//VELOCITY

	//Velocity Circle
	lcd_circle(98, 144, 92, CUSTOM_2); //
	lcd_circle(98, 144, 91, CUSTOM_2); //
	lcd_circle(98, 144, 90, GREEN); //
	lcd_circle(98, 144, 89, CUSTOM_2); //
	lcd_circle(98, 144, 88, CUSTOM_2); //

	lcd_fillcircle(98, 144, 87, LIGHT_GRAY); //

	//THROTTLE

	//Throttle Rectangle
	lcd_drawRect(200, 67, 234, 235, CUSTOM_2); //
	lcd_drawRect(201, 68, 233, 234, CUSTOM_2); //
	lcd_drawRect(202, 69, 232, 233, GREEN); //
	lcd_drawRect(203, 70, 231, 232, CUSTOM_2); //
	lcd_drawRect(204, 71, 230, 231, CUSTOM_2); //

	lcd_fillRect(205, 72, 229, 230, LIGHT_GRAY); //

	//LIGHTRAIL STOPS

	//Current Stop Rectangle
	lcd_drawRect(5, 22, 117, 43, CUSTOM_2); //
	lcd_drawRect(6, 23, 116, 42, CUSTOM_2); //
	lcd_drawRect(7, 24, 115, 41, CYAN); //
	lcd_drawRect(8, 25, 114, 40, CUSTOM_2); //
	lcd_drawRect(9, 26, 113, 39, CUSTOM_2); //

	lcd_fillRect(10, 27, 112, 38, LIGHT_GRAY); //

	//Next Stop Rectangle
	lcd_drawRect(124, 22, 236, 43, CUSTOM_2); //
	lcd_drawRect(125, 23, 235, 42, CUSTOM_2); //
	lcd_drawRect(126, 24, 234, 41, CYAN); //
	lcd_drawRect(127, 25, 233, 40, CUSTOM_2); //
	lcd_drawRect(128, 26, 232, 39, CUSTOM_2); //

	lcd_fillRect(129, 27, 231, 38, LIGHT_GRAY); //

	//BRAKE

	//Brake
	lcd_circle(200, 280, 35, CUSTOM_2); //
	lcd_circle(200, 280, 34, CUSTOM_2); //
	lcd_circle(200, 280, 33, RED); //
	lcd_circle(200, 280, 32, CUSTOM_2); //
	lcd_circle(200, 280, 31, CUSTOM_2); //

	lcd_fillcircle(200, 280, 30, RED); //

	//SAFETY INDICATORS

	//DMS Circle
	lcd_circle(43, 289, 26, CUSTOM_2); //
	lcd_circle(43, 289, 25, CUSTOM_2); //
	lcd_circle(43, 289, 24, CUSTOM_2); //
	lcd_circle(43, 289, 23, YELLOW); //
	lcd_circle(43, 289, 22, CUSTOM_2); //
	lcd_circle(43, 289, 21, CUSTOM_2); //
	lcd_circle(43, 289, 20, CUSTOM_2); //

	// OFF POSITION
	lcd_fillcircle(43, 289, 19, LIGHT_GRAY); //
	// ON POSITION
	lcd_fillcircle(43, 289, 19, YELLOW); //

	//MEX VEL Circle
	lcd_circle(119, 289, 26, CUSTOM_2); //
	lcd_circle(119, 289, 25, CUSTOM_2); //
	lcd_circle(119, 289, 24, CUSTOM_2); //
	lcd_circle(119, 289, 23, YELLOW); //
	lcd_circle(119, 289, 22, CUSTOM_2); //
	lcd_circle(119, 289, 21, CUSTOM_2); //
	lcd_circle(119, 289, 20, CUSTOM_2); //

	// OFF POSITION
	lcd_fillcircle(119, 289, 19, LIGHT_GRAY); //
	// ON POSITION
	lcd_fillcircle(119, 289, 19, YELLOW); //

	/********** NON-STATIC *********/

	//Throttle

	//Velocity

	while (1) {
		//INT TO STRING (returns integer in base 10)
		//Right Shifted 16 bits.
		int ms_vel = light_rail.velocity >> 16;
		int kh_vel = ceil(ms_vel/3.6);

		char str_vel[100];
		itoa(kh_vel, str_vel, 10);
		lcd_putString(unsigned short x, unsigned short y, unsigned char *pStr);

		// DELAY
		if (/*Touchscreen Pushed*/) {
			light_rail.brake = 1;
		} else {
			light_rail.brake = 0;
		}

		if (light_rail.dms == 1) {

		}

	}

	//Do nothing more
	while (1) { }
}

/*
void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x0, unsigned short y0, int thickness, lcd_color_t color) {
	
	for (int i = 0; i < thickness, i++;) {
		lcd_drawRect(x0 + i, y0 + i, x1 - i, y1 - i, color);
	}

}
*/

/*
void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, int thickness, lcd_color_t color) {
	
	for (int i = 0; i < thickness, i++;) {
		lcd_circle(x0 , y0, r - i, color);
	}

}


void lcd_throttle_lever(int throttle_position) {
	int i = 0;
	while () {
		lcd_fillRect(206, 220, 229, 229, DARK_GRAY);
	}
		
}

void lcd_indicator_state(unsigned short x0, unsigned short y0, unsigned short r, int state) {

	if (state == 0) {
		lcd_fillcircle(x0, y0, r, LIGHT_GRAY);
	} else if (state == 1) {
		lcd_fillcircle(x0, y0, r, RED);	
	}
		
}

*/