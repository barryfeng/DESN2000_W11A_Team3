#include "lpc24xx.h"
#include "delay.h"
#include "lcd/lcd_hw.h"
#include "lcd/lcd_grph.h"
#include "lcd/lcd_cfg.h"
#include "lcd/sdram.h"

#include <master_controller.h>

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
	
	//Throttle Rectangle
	lcd_drawRect(204, 72, 232, 232, BLACK);
	lcd_drawRect(205, 73, 231, 231, BLACK);
	lcd_fillRect(206, 74, 230, 230, YELLOW);

	/* NON-CONSTANTS */

	// Velocity

	// Right Shifted 16 bits.
	 int vel = light_rail->velocity; 
	 char svel[2];

	 // INT TO STRING (returns integer in base 10)
	 itoa(vel, svel, 10);

	 lcd_putString(unsigned short x, unsigned short y, unsigned char *pStr);



	//Do nothing more
	while (1) { }
}
