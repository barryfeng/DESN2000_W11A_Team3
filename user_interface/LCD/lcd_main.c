#include "lpc24xx.h"
#include "delay.h"
#include "lcd/lcd_hw.h"
#include "lcd/lcd_grph.h"
#include "lcd/lcd_cfg.h"
#include "lcd/sdram.h"

// EXTRA #INCLUDES
#include <master_controller.h>

// EXTRA #DEFINES
#define OFF 0
#define ON 1

/*
void lcd_throttle_lever(int throttle_position);
void lcd_indicator_state(unsigned short x0, unsigned short y0, unsigned short r, int state);
*/

void constant_borders();
void constant_velocity();
void constant_throttle();
void constant_stops();
void constant_brake();
void constant_safety_indicators();
void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x0, unsigned short y0, int thickness, lcd_color_t color);
void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, int thickness, lcd_color_t color);

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
	void constant_borders();

	//VELOCITY
	void constant_velocity();

	//THROTTLE
	void constant_throttle();

	//STOPS
	void constant_stops();

	//BRAKE
	void constant_brake();

	//SAFETY INDICATORS
	void constant_safety_indicators();

	/********** NON-STATIC *********/

	//Throttle

	//Velocity

	while (1) {
		//INT TO STRING (returns integer in base 10)
		int ms_vel = light_rail.velocity >> 16;
		int kmh_vel = ceil(ms_vel/3.6);

		char str_vel[100];
		itoa(kmh_vel, str_vel, 10);
		lcd_putLargeString(unsigned short x, unsigned short y, unsigned char *pStr);

		/////////////////////////////////
		// TOUCHSCREEEN

		if (/*Touchscreen Pushed*/) {
			light_rail.brake = ON;
		} else {
			light_rail.brake = OFF;
		}
		/////////////////////////////////

		int dms_state = light_rail.dms;

		if (dms_state == OFF) {
			lcd_fillcircle(43, 289, 19, LIGHT_GRAY);
		} else if (dms_state == ON) {
			lcd_fillcircle(43, 289, 19, YELLOW);
		}

		if (kmh_vel < 50) {
			lcd_fillcircle(119, 289, 19, LIGHT_GRAY);	
		} else {
			lcd_fillcircle(119, 289, 19, YELLOW);
		}

		// DELAY (SMALL)

	}

	//Do nothing more
	while (1) { }
}

void constant_borders() {
	//Outer Border
	lcd_drawRect(1, 1, 240, 320, CUSTOM_2);
	//Current Stop Border
	lcd_drawRect(2, 2, 120, 47, CUSTOM_2);
	//Next Stop Border
	lcd_drawRect(121, 2, 239, 47, CUSTOM_2);
	//Velocity Border
	lcd_drawRect(2, 48, 194, 240, CUSTOM_2);
	//Throttle Border
	lcd_drawRect(195, 48, 239, 240, CUSTOM_2);
	//Safety Indicators Border
	lcd_drawRect(2, 241, 160, 319, CUSTOM_2);
	//Brake Border
	lcd_drawRect(161, 241, 239, 319, CUSTOM_2);
}

void constant_velocity() {
	//Velocity Circle
	lcd_circle_thickness(98, 144, 92, 2, CUSTOM_2);
	lcd_circle(98, 144, 90, GREEN);
	lcd_circle_thickness(98, 144, 89, 2, CUSTOM_2);
	lcd_fillcircle(98, 144, 87, LIGHT_GRAY);
	//Velocity Label
	//putVelString(x,y,"km/h");
}

void constant_throttle() {
	//Throttle Label
	lcd_rectangle_thickness(198, 52, 236, 64, 2, CUSTOM_2);
	lcd_fillRect(200, 54, 234, 62, LIGHT_GRAY);
	//putString(x,y,"THROTTLE");

	//Throttle Rectangle
	lcd_rectangle_thickness(200, 67, 234, 235, 2, CUSTOM_2);
	lcd_drawRect(202, 69, 232, 233, MAGENTA);
	lcd_rectangle_thickness(203, 70, 231, 232, 2, CUSTOM_2);
	lcd_fillRect(205, 72, 229, 230, LIGHT_GRAY);
}

void constant_stops() {
	// Current Stop Label
	lcd_rectangle_thickness(7, 6, 115, 18, 2, CUSTOM_2);
	lcd_fillRect(9, 8, 113, 16, LIGHT_GRAY);
	//putString(x,y,"CURRENT STOP");

	//Current Stop Rectangle
	lcd_rectangle_thickness(5, 23, 117, 43, 2, CUSTOM_2);
	lcd_drawRect(7, 25, 115, 41, CYAN);
	lcd_rectangle_thickness(8, 26, 114, 40, 2, CUSTOM_2);
	lcd_fillRect(10, 28, 112, 38, LIGHT_GRAY);

	// Next Stop Label
	lcd_rectangle_thickness(126, 6, 234, 18, 2, CUSTOM_2);
	lcd_fillRect(128, 8, 232, 16, LIGHT_GRAY);
	//putString(x,y,"NEXT STOP");
	
	//Next Stop Rectangle
	lcd_rectangle_thickness(124, 23, 236, 43, 2, CUSTOM_2);
	lcd_drawRect(126, 25, 234, 41, CYAN);
	lcd_rectangle_thickness(127, 26, 233, 40, 2, CUSTOM_2);
	lcd_fillRect(129, 28, 231, 38, LIGHT_GRAY);
}

void constant_brake() {
	//Brake Circle
	lcd_circle_thickness(200, 280, 35, 2, CUSTOM_2);
	lcd_circle(200, 280, 33, RED);
	lcd_circle_thickness(200, 280, 32, 2, CUSTOM_2);
	lcd_fillcircle(200, 280, 30, RED);

	// Brake Label
	//putBrakeString(x,y,"Brake");
}

void constant_safety_indicators() {
	//DMS Circle
	lcd_circle_thickness(43, 289, 26, 3, CUSTOM_2);
	lcd_circle(43, 289, 23, YELLOW);
	lcd_circle_thickness(43, 289, 22, 3, CUSTOM_2);

	// DMS Label
	lcd_rectangle_thickness(17, 245, 69, 257, 2, CUSTOM_2);
	lcd_fillRect(19, 247, 67, 255, LIGHT_GRAY);
	//putString(x,y,"DMS");

	//MAX VEL Circle
	lcd_circle_thickness(119, 289, 26, 3, CUSTOM_2);
	lcd_circle(119, 289, 23, YELLOW);
	lcd_circle_thickness(119, 289, 22, 3, CUSTOM_2);

	// MAX VEL Label
	lcd_rectangle_thickness(93, 245, 145, 257, 2, CUSTOM_2);
	lcd_fillRect(95, 247, 143, 255, LIGHT_GRAY);
	//putString(x,y,"MAX VEL");
}

void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x0, unsigned short y0, int thickness, lcd_color_t color) {
	
	for (int i = 0; i < thickness, i++;) {
		lcd_drawRect(x0 + i, y0 + i, x1 - i, y1 - i, color);
	}

}

void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, int thickness, lcd_color_t color) {
	
	for (int i = 0; i < thickness, i++;) {
		lcd_circle(x0 , y0, r - i, color);
	}

}

/*

void lcd_throttle_lever(int throttle_position) {
	int i = 0;
	while () {
		lcd_fillRect(206, 220, 229, 229, DARK_GRAY);
	}
		
}

*/