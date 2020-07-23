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
	lcd_fillScreen(DARK_CYAN);

	/* CONSTANTS */

	//Borders

	//Outer Border
	lcd_drawRect(1, 1, 240, 360, BLACK);
	//Current Stop Border
	lcd_drawRect(1, 1, 120, 48, BLACK);
	//Next Stop Border
	lcd_drawRect(120, 1, 240, 48, BLACK);
	//Velocity Border
	lcd_drawRect(1, 48, 196, 240, BLACK);
	//Throttle Border
	lcd_drawRect(196, 48, 240, 240, BLACK);
	//Safety Indicators Border
	lcd_drawRect(1, 240, 160, 320, BLACK);
	//Brake Border
	lcd_drawRect(160, 240, 240, 360, BLACK);

	//Velocity

	//Velocity Circle
	lcd_circle(100, 144, 92, BLACK);
	lcd_circle(100, 144, 91, BLACK);
	lcd_fillcircle(100, 144, 90, YELLOW);
	
	//Throttle

	//Throttle Rectangle
	lcd_drawRect(204, 72, 232, 232, BLACK);
	lcd_drawRect(205, 73, 231, 231, BLACK);
	lcd_fillRect(206, 74, 230, 230, YELLOW);

	//Current Stop
	lcd_drawRect(12, 28, 112, 40, BLACK);
	lcd_drawRect(13, 29, 111, 39, BLACK);

	//Next Stop
	lcd_drawRect(128, 28, 232, 40, BLACK);
	lcd_drawRect(129, 29, 231, 39, BLACK);

	//Brake
	lcd_circle(200, 280, 36, BLACK);
	lcd_circle(200, 280, 35, BLACK);
	// OFF POSITION
	lcd_fillcircle(200, 280, 34, LIGHT_GRAY);
	// ON POSITION
	lcd_fillcircle(200, 280, 34, RED);
	// BRAKE TEXT


	//Safety LED's

	//DMS Circle
	lcd_circle(44, 288, 28, BLACK);
	lcd_circle(44, 288, 27, BLACK);
	// OFF POSITION
	lcd_fillcircle(44, 288, 26, LIGHT_GRAY);
	// ON POSITION
	lcd_fillcircle(44, 288, 26, RED);

	//Max Velocity Circle
	lcd_circle(120, 288, 28, BLACK);
	lcd_circle(120, 288, 27, BLACK);
	// OFF POSITION
	lcd_fillcircle(120, 288, 26, LIGHT_GRAY);
	// ON POSITION
	lcd_fillcircle(44, 288, 26, RED);

	/* NON-CONSTANTS */

	//Throttle 

	//Velocity

	//Right Shifted 16 bits.
	 int vel = light_rail->velocity; 
	 char svel[2];

	 //INT TO STRING (returns integer in base 10)
	 itoa(vel, svel, 10);

	 lcd_putString(unsigned short x, unsigned short y, unsigned char *pStr);



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