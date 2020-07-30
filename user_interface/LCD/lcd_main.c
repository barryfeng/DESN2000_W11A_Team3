#include "lcd_main.h"

void lcd_start() {	//DO NOT REMOVE FROM THIS FILE
	//Setup external SDRAM. Used for Frame Buffer
	sdramInit();
	
	//Setup LPC2478 LCD Controller for our specific LCD
	//lcd_config is defined in lcd/lcd_cfg.h
	lcdInit(&lcd_config);
	
	//Turn the LCD on
	lcdTurnOn();
	
	//Draw a background with lcd_fillScreen
	lcd_fillScreen(CUSTOM_1);

	//BORDERS
	constant_borders();
}

void lcd_run() {	//DO NOT REMOVE FROM THIS FILE
	int vel_ms = light_rail.velocity >> 16;
	int vel_kmh = ceil(vel_ms/3.6);
	int dms_state = light_rail.dms;
	int brake_state = light_rail.brake;

	char vel_str[100];
	//INT TO STRING (returns integer in base 10)
	itoa(vel_kmh, vel_str, 10);

	if (dms_state == ON || (vel_kmh == STOP && brake_state == APPLY)) {
		//VELOCITY
		constant_velocity_off();
		//THROTTLE
		constant_throttle_off();
		//STOPS
		constant_stops_off();
		//BRAKE
		constant_brake_off();
		//SD
		constant_sd_off();
	} else {
		//VELOCITY
		constant_velocity_on();
		//THROTTLE
		constant_throttle_on();
		//STOPS
		constant_stops_on();
		//BRAKE
		constant_brake_on();
		//SD
		constant_sd_on();
	}

	//DMS
	if (dms_state == OFF) {
		constant_dms_off();
	} else if (dms_state == ON) {
		constant_dms_on();
	}

	//MAX VEL
	if (vel_kmh < MAX_VEL) {
		constant_max_vel_off();	
	} else {
		constant_max_vel_on();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////

void constant_velocity_on() {
	//Velocity Circle
	lcd_circle_thickness(98, 144, 92, 2, CUSTOM_2);
	lcd_circle(98, 144, 90, GREEN);
	lcd_circle_thickness(98, 144, 89, 2, CUSTOM_2);
	lcd_fillcircle(98, 144, 87, GREEN);
	//Velocity Label
	//putVelString(x,y,"positive km/h");
}

void constant_throttle_on() {
	//Throttle Label
	lcd_rectangle_thickness(198, 52, 236, 64, 2, CUSTOM_2);
	lcd_fillRect(200, 54, 234, 62, LIGHT_GRAY);
	//putString(x,y,"THROTTLE");

	//Throttle Rectangle
	lcd_rectangle_thickness(200, 67, 234, 235, 2, CUSTOM_2);
	lcd_drawRect(202, 69, 232, 233, MAGENTA);
	lcd_rectangle_thickness(203, 70, 231, 232, 2, CUSTOM_2);
	lcd_fillRect(205, 72, 229, 230, MAGENTA);
}

void constant_stops_on() {
	// Current Stop Label
	lcd_rectangle_thickness(7, 6, 115, 18, 2, CUSTOM_2);
	lcd_fillRect(9, 8, 113, 16, LIGHT_GRAY);
	//putString(x,y,"CURRENT STOP");

	//Current Stop Rectangle
	lcd_rectangle_thickness(5, 23, 117, 43, 2, CUSTOM_2);
	lcd_drawRect(7, 25, 115, 41, CYAN);
	lcd_rectangle_thickness(8, 26, 114, 40, 2, CUSTOM_2);
	lcd_fillRect(10, 28, 112, 38, CYAN);
	//putString(x,y,"current stop location");

	// Next Stop Label
	lcd_rectangle_thickness(126, 6, 234, 18, 2, CUSTOM_2);
	lcd_fillRect(128, 8, 232, 16, LIGHT_GRAY);
	//putString(x,y,"NEXT STOP");
	
	//Next Stop Rectangle
	lcd_rectangle_thickness(124, 23, 236, 43, 2, CUSTOM_2);
	lcd_drawRect(126, 25, 234, 41, CYAN);
	lcd_rectangle_thickness(127, 26, 233, 40, 2, CUSTOM_2);
	lcd_fillRect(129, 28, 231, 38, CYAN);
	//putString(x,y,"next stop location");
}

void constant_brake_on() {
	//Brake Circle
	lcd_circle_thickness(200, 280, 35, 2, CUSTOM_2);
	lcd_circle(200, 280, 33, RED);
	lcd_circle_thickness(200, 280, 32, 2, CUSTOM_2);
	lcd_fillcircle(200, 280, 30, RED);

	// Brake Label
	//putBrakeString(x,y,"Brake");
}

void constant_dms_on() {
	// DMS Label
	lcd_rectangle_thickness(46, 246, 98, 258, 2, CUSTOM_2);
	lcd_fillRect(48, 248, 96, 256, LIGHT_GRAY);
	//putString(x,y,"DMS");
	
	//DMS Circle
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_circle(72, 289, 26, YELLOW);
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_fillcircle(72, 289, 19, YELLOW);
}

void constant_max_vel_on() {
	// MAX VEL Label
	lcd_rectangle_thickness(103, 246, 155, 258, 2, CUSTOM_2);
	lcd_fillRect(105, 248, 153, 256, LIGHT_GRAY);
	//putString(x,y,"MAX VEL");

	//MAX VEL Circle
	lcd_circle_thickness(129, 289, 26, 3, CUSTOM_2);
	lcd_circle(129, 289, 23, YELLOW);
	lcd_circle_thickness(129, 289, 22, 3, CUSTOM_2);
	lcd_fillcircle(129, 289, 19, YELLOW);
}

void constant_sd_on() {
	//SD Label
	lcd_rectangle_thickness(7, 246, 39, 258, 2, CUSTOM_2);
	lcd_fillRect(9, 248, 37, 256, LIGHT_GRAY);
	//putString(x,y,"SD");

	//SD Rectangle
	lcd_rectangle_thickness(7, 264, 39, 314, 2, CUSTOM_2);
	lcd_drawRect(9, 266, 37, 312, WHITE);
	lcd_rectangle_thickness(10, 267, 36, 311, 2, CUSTOM_2);
	lcd_fillRect(12, 269, 34, 309, WHITE);
}

////////////////////////////////////////////////////////////////////////////////////////

void constant_velocity_off() {
	//Velocity Circle
	lcd_circle_thickness(98, 144, 92, 2, CUSTOM_2);
	lcd_circle(98, 144, 90, LIGHT_GRAY);
	lcd_circle_thickness(98, 144, 89, 2, CUSTOM_2);
	lcd_fillcircle(98, 144, 87, LIGHT_GRAY);
	//Velocity Label
	//putVelString(x,y,"0 km/h");
}

void constant_throttle_off() {
	//Throttle Label
	lcd_rectangle_thickness(198, 52, 236, 64, 2, CUSTOM_2);
	lcd_fillRect(200, 54, 234, 62, LIGHT_GRAY);
	//putString(x,y,"THROTTLE");

	//Throttle Rectangle
	lcd_rectangle_thickness(200, 67, 234, 235, 2, CUSTOM_2);
	lcd_drawRect(202, 69, 232, 233, LIGHT_GRAY);
	lcd_rectangle_thickness(203, 70, 231, 232, 2, CUSTOM_2);
	lcd_fillRect(205, 72, 229, 230, LIGHT_GRAY);
}

void constant_stops_off() {
	// Current Stop Label
	lcd_rectangle_thickness(7, 6, 115, 18, 2, CUSTOM_2);
	lcd_fillRect(9, 8, 113, 16, LIGHT_GRAY);
	//putString(x,y,"CURRENT STOP");

	//Current Stop Rectangle
	lcd_rectangle_thickness(5, 23, 117, 43, 2, CUSTOM_2);
	lcd_drawRect(7, 25, 115, 41, LIGHT_GRAY);
	lcd_rectangle_thickness(8, 26, 114, 40, 2, CUSTOM_2);
	lcd_fillRect(10, 28, 112, 38, LIGHT_GRAY);
	//putString(x,y,"empty string");

	// Next Stop Label
	lcd_rectangle_thickness(126, 6, 234, 18, 2, CUSTOM_2);
	lcd_fillRect(128, 8, 232, 16, LIGHT_GRAY);
	//putString(x,y,"NEXT STOP");
	
	//Next Stop Rectangle
	lcd_rectangle_thickness(124, 23, 236, 43, 2, CUSTOM_2);
	lcd_drawRect(126, 25, 234, 41, LIGHT_GRAY);
	lcd_rectangle_thickness(127, 26, 233, 40, 2, CUSTOM_2);
	lcd_fillRect(129, 28, 231, 38, LIGHT_GRAY);
	//putString(x,y,"empty string");
}

void constant_brake_off() {
	//Brake Circle
	lcd_circle_thickness(200, 280, 35, 2, CUSTOM_2);
	lcd_circle(200, 280, 33, LIGHT_GRAY);
	lcd_circle_thickness(200, 280, 32, 2, CUSTOM_2);
	lcd_fillcircle(200, 280, 30, LIGHT_GRAY);

	// Brake Label
	//putBrakeString(x,y,"Brake");
}

void constant_dms_off() {
	// DMS Label
	lcd_rectangle_thickness(46, 246, 98, 258, 2, CUSTOM_2);
	lcd_fillRect(48, 248, 96, 256, LIGHT_GRAY);
	//putString(x,y,"DMS");
	
	//DMS Circle
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_circle(72, 289, 26, LIGHT_GRAY);
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_fillcircle(72, 289, 19, LIGHT_GRAY);
}

void constant_max_vel_off() {
	// MAX VEL Label
	lcd_rectangle_thickness(103, 246, 155, 258, 2, CUSTOM_2);
	lcd_fillRect(105, 248, 153, 256, LIGHT_GRAY);
	//putString(x,y,"MAX VEL");

	//MAX VEL Circle
	lcd_circle_thickness(129, 289, 26, 3, CUSTOM_2);
	lcd_circle(129, 289, 23, LIGHT_GRAY);
	lcd_circle_thickness(129, 289, 22, 3, CUSTOM_2);
	lcd_fillcircle(129, 289, 19, LIGHT_GRAY);
}

void constant_sd_off() {
	//SD Label
	lcd_rectangle_thickness(7, 246, 39, 258, 2, CUSTOM_2);
	lcd_fillRect(9, 248, 37, 256, LIGHT_GRAY);
	//putString(x,y,"SD");

	//SD Rectangle
	lcd_rectangle_thickness(7, 264, 39, 314, 2, CUSTOM_2);
	lcd_drawRect(9, 266, 37, 312, LIGHT_GRAY);
	lcd_rectangle_thickness(10, 267, 36, 311, 2, CUSTOM_2);
	lcd_fillRect(12, 269, 34, 309, LIGHT_GRAY);
}

///////////////////////////////////////////////////////////////////////////////////////////

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