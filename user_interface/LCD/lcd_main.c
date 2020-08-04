#include "lcd_main.h"

extern LightRail light_rail;

void init_lcd() {	//DO NOT REMOVE FROM THIS FILE
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

	//LABELS
	constant_labels();
}

void lcd_run() {	//DO NOT REMOVE FROM THIS FILE

	//CALCULATE VELOCITY 
	int vel_ms = light_rail.velocity >> 16;
	int vel_kmh = vel_ms * 3.6;

	//LCD BACKLIGHT
	if (vel_kmh == STOP && light_rail.brake_state == APPLY) {
		lcd_backlight_off();
	} else {
		lcd_backlight_on();
	}

	//INT TO STRING (returns integer in base 10)
	char vel_str[100];
	itoa(vel_kmh, vel_str, 10);
	//lcd_putVelString(x, y, vel_str);

	//DMS STATE CHECK
	if (light_rail.dms_state == OFF) {
		constant_dms_off();
	} else if (light_rail.dms_state == ON) {
		constant_dms_on();
	}

	//MAX VEL STATE CHECK
	if (vel_kmh < MAX_VEL) {
		constant_max_vel_off();	
	} else {
		constant_max_vel_on();
	}

	int i = 0;
    char light_rail_stop[MAX_STOPS][MAX_STOP_STRING_SIZE] = {"CENTRAL", "HAYMARKET", "CHINATOWN", "TOWN HALL", "QVB", "WYNYARD", "BRIDGE ST", "CIRC QUAY"};

	//CURRENT/NEXT STOP CHECK
	if (light_rail.brake_state == APPLY && light_rail_stop[i + 1] != '\0') {
		//lcd_putString(x, y, light_rail_stop[i]);
		//lcd_putString(x, y, light_rail_stop[i + 1]);
		i++;
	} else if (light_rail.brake_state == APPLY && light_rail_stop[i + 1] == '\0') {

		for (int i = 0; i < MAX_STOPS - 1; i++) {
			for (int j = 0, k = strlen(light_rail_stop[i + 1]) - 1; j < k; j++, k--) {
				char temp = light_rail_stop[i + 1][j];
				light_rail_stop[i + 1][j] = light_rail_stop[i + 1][k];
				light_rail_stop[i + 1][k] = temp;
			}
		}
		//lcd_putString(x, y, light_rail_stop[i]);
		//lcd_putString(x, y, light_rail_stop[i + 1]);
	} else {
		//lcd_putString(x, y, light_rail_stop[i]);
		//lcd_putString(x, y, light_rail_stop[i + 1]);	
	}

}

///////////////////////////////////
///*    LCD START CONSTANTS    *///
///////////////////////////////////

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

void constant_labels() {
	//Throttle Label
	lcd_rectangle_thickness(198, 52, 236, 64, 2, CUSTOM_2);
	lcd_fillRect(200, 54, 234, 62, LIGHT_GRAY);
	//putString(x,y,"THROT");

	// Current Stop Label
	lcd_rectangle_thickness(7, 6, 115, 18, 2, CUSTOM_2);
	lcd_fillRect(9, 8, 113, 16, LIGHT_GRAY);
	//putString(x,y,"CURRENT STOP");

	// Next Stop Label
	lcd_rectangle_thickness(126, 6, 234, 18, 2, CUSTOM_2);
	lcd_fillRect(128, 8, 232, 16, LIGHT_GRAY);
	//putString(x,y,"NEXT STOP");

	// DMS Label
	lcd_rectangle_thickness(46, 246, 98, 258, 2, CUSTOM_2);
	lcd_fillRect(48, 248, 96, 256, LIGHT_GRAY);
	//putString(x,y,"DMS");

	// MAX VEL Label
	lcd_rectangle_thickness(103, 246, 155, 258, 2, CUSTOM_2);
	lcd_fillRect(105, 248, 153, 256, LIGHT_GRAY);
	//putString(x,y,"MAX VEL");

	//SD Label
	lcd_rectangle_thickness(7, 246, 39, 258, 2, CUSTOM_2);
	lcd_fillRect(9, 248, 37, 256, LIGHT_GRAY);
	//putString(x,y,"SD");
}

/////////////////////////////
///*    LCD BACKLIGHT    *///
/////////////////////////////

void lcd_backlight_on() {
	//VELOCITY
	constant_velocity_on();

	//THROTTLE
	constant_throttle_plus_on();
	constant_throttle_minus_on();

	//STOPS
	constant_stops_on();

	//BRAKE
	constant_brake_on();

	//SD
	constant_sd_on();
}

void lcd_backlight_off() {
	//VELOCITY
	constant_velocity_off();

	//THROTTLE
	constant_throttle_plus_off();
	constant_throttle_minus_off();

	//STOPS
	constant_stops_off();

	//BRAKE
	constant_brake_off();

	//SD
	constant_sd_off();
}

//////////////////////////////////////
///*    LCD RUN CONSTANTS (ON)    *///
//////////////////////////////////////

void constant_velocity_on() {
	//Velocity Circle
	lcd_circle_thickness(98, 144, 92, 2, CUSTOM_2);
	lcd_circle(98, 144, 90, GREEN);
	lcd_circle_thickness(98, 144, 89, 2, CUSTOM_2);
	lcd_fillcircle(98, 144, 87, GREEN);

	//Velocity Label
	//putVelLabelString(x,y,"KM/H");
}

void constant_throttle_plus_on() {
	//Throttle Rectangle
	lcd_rectangle_thickness(200, 68, 234, 149, 2, CUSTOM_2);
	lcd_drawRect(202, 70, 232, 147, MAGENTA);
	lcd_rectangle_thickness(203, 71, 231, 146, 2, CUSTOM_2);
	lcd_fillRect(205, 73, 229, 144, MAGENTA);

	//Plus Symbol
	lcd_fillRect(216, 98, 218, 119, CUSTOM_2);
	lcd_fillRect(208, 107, 226, 110, CUSTOM_2);
}

void constant_throttle_minus_on() {
	//Throttle Rectangle
	lcd_rectangle_thickness(200, 154, 234, 235, 2, CUSTOM_2);
	lcd_drawRect(202, 156, 232, 233, MAGENTA);
	lcd_rectangle_thickness(203, 157, 231, 232, 2, CUSTOM_2);
	lcd_fillRect(205, 159, 229, 230, MAGENTA);

	//Minus Symbol
	lcd_fillRect(208, 193, 226, 196, CUSTOM_2);
}

void constant_stops_on() {
	//Current Stop Rectangle
	lcd_rectangle_thickness(5, 23, 117, 43, 2, CUSTOM_2);
	lcd_drawRect(7, 25, 115, 41, CYAN);
	lcd_rectangle_thickness(8, 26, 114, 40, 2, CUSTOM_2);
	lcd_fillRect(10, 28, 112, 38, CYAN);
	//putString(x,y,"current stop location");
	
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
	//putBrakeLabelString(x,y,"BRAKE");
}

void constant_dms_on() {
	//DMS Circle
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_circle(72, 289, 26, YELLOW);
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_fillcircle(72, 289, 19, YELLOW);
}

void constant_max_vel_on() {
	//MAX VEL Circle
	lcd_circle_thickness(129, 289, 26, 3, CUSTOM_2);
	lcd_circle(129, 289, 23, YELLOW);
	lcd_circle_thickness(129, 289, 22, 3, CUSTOM_2);
	lcd_fillcircle(129, 289, 19, YELLOW);
}

void constant_sd_on() {
	//SD Rectangle
	lcd_rectangle_thickness(7, 264, 39, 314, 2, CUSTOM_2);
	lcd_drawRect(9, 266, 37, 312, WHITE);
	lcd_rectangle_thickness(10, 267, 36, 311, 2, CUSTOM_2);
	lcd_fillRect(12, 269, 34, 309, WHITE);

	//SD Symbol
	lcd_fillRect(16, 277, 30, 305, CUSTOM_2);
	for (int i = 0; i < 4, i++;) {
		lcd_fillRect(17 + i, 276 - i, 30, 276 - i, CUSTOM_2);
	}
	for (int i = 0; i < 5, i++;) {
		lcd_fillRect(21 + 2*i, 274, 21 + 2*i, 276, YELLOW);
	}
}

///////////////////////////////////////
///*    LCD RUN CONSTANTS (OFF)    *///
///////////////////////////////////////

void constant_velocity_off() {
	//Velocity Circle
	lcd_circle_thickness(98, 144, 92, 2, CUSTOM_2);
	lcd_circle(98, 144, 90, LIGHT_GRAY);
	lcd_circle_thickness(98, 144, 89, 2, CUSTOM_2);
	lcd_fillcircle(98, 144, 87, LIGHT_GRAY);

	//Velocity Label
	//putVelLabelString(x,y,"KM/H");
}

void constant_throttle_plus_off() {
	//Throttle Rectangle
	lcd_rectangle_thickness(200, 68, 234, 149, 2, CUSTOM_2);
	lcd_drawRect(202, 70, 232, 147, MAGENTA);
	lcd_rectangle_thickness(203, 71, 231, 146, 2, CUSTOM_2);
	lcd_fillRect(205, 73, 229, 144, MAGENTA);

	//Plus Symbol
	lcd_fillRect(216, 98, 218, 119, CUSTOM_2);
	lcd_fillRect(208, 107, 226, 110, CUSTOM_2);
}

void constant_throttle_minus_off() {
	//Throttle Rectangle
	lcd_rectangle_thickness(200, 154, 234, 235, 2, CUSTOM_2);
	lcd_drawRect(202, 156, 232, 233, LIGHT_GRAY);
	lcd_rectangle_thickness(203, 157, 231, 232, 2, CUSTOM_2);
	lcd_fillRect(205, 159, 229, 230, LIGHT_GRAY);

	//Minus Symbol
	lcd_fillRect(208, 193, 226, 196, CUSTOM_2);
}

void constant_stops_off() {
	//Current Stop Rectangle
	lcd_rectangle_thickness(5, 23, 117, 43, 2, CUSTOM_2);
	lcd_drawRect(7, 25, 115, 41, LIGHT_GRAY);
	lcd_rectangle_thickness(8, 26, 114, 40, 2, CUSTOM_2);
	lcd_fillRect(10, 28, 112, 38, LIGHT_GRAY);
	//putString(x,y,"empty string");
	
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
	//putBrakeLabelString(x,y,"BRAKE");
}

void constant_dms_off() {
	//DMS Circle
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_circle(72, 289, 26, LIGHT_GRAY);
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_fillcircle(72, 289, 19, LIGHT_GRAY);
}

void constant_max_vel_off() {
	//MAX VEL Circle
	lcd_circle_thickness(129, 289, 26, 3, CUSTOM_2);
	lcd_circle(129, 289, 23, LIGHT_GRAY);
	lcd_circle_thickness(129, 289, 22, 3, CUSTOM_2);
	lcd_fillcircle(129, 289, 19, LIGHT_GRAY);
}

void constant_sd_off() {
	//SD Rectangle
	lcd_rectangle_thickness(7, 264, 39, 314, 2, CUSTOM_2);
	lcd_drawRect(9, 266, 37, 312, LIGHT_GRAY);
	lcd_rectangle_thickness(10, 267, 36, 311, 2, CUSTOM_2);
	lcd_fillRect(12, 269, 34, 309, LIGHT_GRAY);

	//SD Symbol
	lcd_fillRect(16, 277, 30, 305, CUSTOM_2);
	for (int i = 0; i < 4, i++;) {
		lcd_fillRect(17 + i, 276 - i, 30, 276 - i, CUSTOM_2);
	}
	for (int i = 0; i < 5, i++;) {
		lcd_fillRect(21 + 2*i, 274, 21 + 2*i, 276, LIGHT_GRAY);
	}
}

///////////////////////////////////
///*    LCD SHAPE THICKNESS    *///
///////////////////////////////////

void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, int thickness, lcd_color_t color) {

	for (int i = 0; i < thickness, i++;) {
		lcd_drawRect(x0 + i, y0 + i, x1 - i, y1 - i, color);
	}

}

void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, int thickness, lcd_color_t color) {
	
	for (int i = 0; i < thickness, i++;) {
		lcd_circle(x0 , y0, r - i, color);
	}

}