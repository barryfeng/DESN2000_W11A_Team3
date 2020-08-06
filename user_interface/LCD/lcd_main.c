#include "lcd_main.h"

extern LightRail light_rail;

///////////////////////////
///*    LCD STARTUP    *///
///////////////////////////

void init_lcd() {
	// Setup external SDRAM. Used for Frame Buffer.
	sdramInit();
	
	// Setup LPC2478 LCD Controller for our specific LCD
	// lcd_config is defined in lcd/lcd_cfg.h.
	lcdInit(&lcd_config);
	
	// Turn the LCD on.
	lcdTurnOn();
	
	// Draw a background with lcd_fillScreen.
	lcd_fillScreen(CUSTOM_1);

	// Draw borders around LCD features.
	lcd_borders();

	// Draw labels above LCD features.
	lcd_labels();
}

///////////////////////
///*    LCD RUN    *///
///////////////////////

void lcd_run() {

	// Converts velocity from Q22 notation to m/s and then to km/h.
	int vel_ms = light_rail.velocity >> 16;
	int vel_kmh = vel_ms * 3.6;

	// Contains velocity string.
	unsigned char vel_str[100]; 

	// Contains counter and list of stops for current and next stop.
	static int i = 0; 
	unsigned char light_rail_stop[MAX_STOPS][MAX_STOP_STRING_SIZE] =
	{"CENTRAL", "HAYMARKET", "CHINATOWN", "TOWN HALL", "QVB", "WYNYARD", "BRIDGE ST", "CIRC QUAY"};

	// Checks LCD mode (standby or active).
	if (vel_kmh == STOP && light_rail.brake_state == APPLY) {
		lcd_standby_backlight_off();
	} else {
		lcd_active_backlight_on();
	}

	// Turns on SD backlight.
	lcd_sd_on();

	// Converts velocity in km/h from an integer to a string (in base 10).
	integer_to_string(vel_kmh, vel_str, 10);
	lcd_putVelString(43, 97, vel_str);

	// Checks dead man's switch (DMS) state.
	if (light_rail.dms_state == OFF) {
		lcd_dms_off();
	} else if (light_rail.dms_state == ON) {
		lcd_dms_on();
	}

	// Checks maximum velocity (MAX VEL) state.
	if (vel_kmh < MAX_VEL) {
		lcd_max_vel_off();	
	} else {
		lcd_max_vel_on();
	}

	// Checks current and next stop location. The list of stops iterates once the 
	// brake is applied and reverses when the end of the list is reached. 
	if (light_rail.brake_state == APPLY && light_rail_stop[i + 1] != '\0') {
		lcd_putString(13, 30, light_rail_stop[i]);
		lcd_putString(131, 30, light_rail_stop[i + i]);
		i++;
	} else if (light_rail.brake_state == APPLY && light_rail_stop[i + 1] == '\0') {
		i = 0;
		while (i < MAX_STOPS - 1) {
			int j = 0, k = strlen((char *)light_rail_stop[i + 1]) - 1;
			while (j < k) {
				char temp = light_rail_stop[i + 1][j];
				light_rail_stop[i + 1][j] = light_rail_stop[i + 1][k];
				light_rail_stop[i + 1][k] = temp;
				j++; 
				k--;
			}
			i++;
		}
		lcd_putString(13, 30, light_rail_stop[i]);
		lcd_putString(131, 30, light_rail_stop[i + i]);
	} else {
		lcd_putString(13, 30, light_rail_stop[i]);
		lcd_putString(131, 30, light_rail_stop[i + i]);	
	}

}

///////////////////////////////////
///*    LCD STARTUP STATICS    *///
///////////////////////////////////

void lcd_borders() {
	// Outer border.
	lcd_drawRect(1, 1, 240, 320, CUSTOM_2);
	// Current stop border.
	lcd_drawRect(2, 2, 120, 47, CUSTOM_2);
	// Next stop border.
	lcd_drawRect(121, 2, 239, 47, CUSTOM_2);
	// Velocity border.
	lcd_drawRect(2, 48, 194, 240, CUSTOM_2);
	// Throttle border.
	lcd_drawRect(195, 48, 239, 240, CUSTOM_2);
	// Safety indicators + SD border.
	lcd_drawRect(2, 241, 160, 319, CUSTOM_2);
	// Brake border.
	lcd_drawRect(161, 241, 239, 319, CUSTOM_2);
}

void lcd_labels() {
	// Throttle label.
	lcd_rectangle_thickness(198, 52, 236, 64, 2, CUSTOM_2);
	lcd_fillRect(200, 54, 234, 62, LIGHT_GRAY);
	lcd_putString(201, 55, "THRT");
	// Current stop label.
	lcd_rectangle_thickness(7, 6, 115, 18, 2, CUSTOM_2);
	lcd_fillRect(9, 8, 113, 16, LIGHT_GRAY);
	lcd_putString(13, 9, "CURRENT STOP");
	// Next stop label.
	lcd_rectangle_thickness(126, 6, 234, 18, 2, CUSTOM_2);
	lcd_fillRect(128, 8, 232, 16, LIGHT_GRAY);
	lcd_putString(144, 9, "NEXT STOP");
	// DMS label.
	lcd_rectangle_thickness(46, 246, 98, 258, 2, CUSTOM_2);
	lcd_fillRect(48, 248, 96, 256, LIGHT_GRAY);
	lcd_putString(60, 249, "DMS");
	// MAX VEL label.
	lcd_rectangle_thickness(103, 246, 155, 258, 2, CUSTOM_2);
	lcd_fillRect(105, 248, 153, 256, LIGHT_GRAY);
	lcd_putString(109, 249, "MAX V");
	// SD label.
	lcd_rectangle_thickness(7, 246, 39, 258, 2, CUSTOM_2);
	lcd_fillRect(9, 248, 37, 256, LIGHT_GRAY);
	lcd_putString(15, 249, "SD");
}

////////////////////////
///*    LCD MODE    *///
////////////////////////

void lcd_active_backlight_on() {
	// Velocity - ON state.
	lcd_velocity_on();
	// Throttle (+) - ON state.
	lcd_throttle_plus_on();
	// Throttle (-) - ON state.
	lcd_throttle_minus_on();
	// Current and next stop - ON state.
	lcd_stops_on();
	// Brake - ON state.
	lcd_brake_on();
}

void lcd_standby_backlight_off() {
	// Velocity - OFF state.
	lcd_velocity_off();
	// Throttle (+) - OFF state.
	lcd_throttle_plus_off();
	// Throttle (-) - OFF state.
	lcd_throttle_minus_off();
	// Current and next stop - OFF state.
	lcd_stops_off();
	// Brake - OFF state.
	lcd_brake_off();
}

////////////////////////////////////
///*    LCD RUN STATICS (ON)    *///
////////////////////////////////////

void lcd_velocity_on() {
	// Velocity circle - ON state.
	lcd_circle_thickness(98, 144, 92, 2, CUSTOM_2);
	lcd_circle(98, 144, 90, GREEN);
	lcd_circle_thickness(98, 144, 89, 2, CUSTOM_2);
	lcd_fillcircle(98, 144, 87, GREEN);
	// Velocity label.
	lcd_putVelLabelString(62, 182, "KM/H");
}

void lcd_throttle_plus_on() {
	// Throttle (+) rectangle - ON state.
	lcd_rectangle_thickness(200, 68, 234, 149, 2, CUSTOM_2);
	lcd_drawRect(202, 70, 232, 147, MAGENTA);
	lcd_rectangle_thickness(203, 71, 231, 146, 2, CUSTOM_2);
	lcd_fillRect(205, 73, 229, 144, MAGENTA);
	// Plus symbol.
	lcd_fillRect(216, 98, 218, 119, CUSTOM_2);
	lcd_fillRect(208, 107, 226, 110, CUSTOM_2);
}

void lcd_throttle_minus_on() {
	// Throttle (-) rectangle - ON state.
	lcd_rectangle_thickness(200, 154, 234, 235, 2, CUSTOM_2);
	lcd_drawRect(202, 156, 232, 233, MAGENTA);
	lcd_rectangle_thickness(203, 157, 231, 232, 2, CUSTOM_2);
	lcd_fillRect(205, 159, 229, 230, MAGENTA);
	// Minus symbol.
	lcd_fillRect(208, 193, 226, 196, CUSTOM_2);
}

void lcd_stops_on() {
	// Current stop rectangle -  ON state.
	lcd_rectangle_thickness(5, 23, 117, 43, 2, CUSTOM_2);
	lcd_drawRect(7, 25, 115, 41, CYAN);
	lcd_rectangle_thickness(8, 26, 114, 40, 2, CUSTOM_2);
	lcd_fillRect(10, 28, 112, 38, CYAN);
	// Next stop rectangle - ON state.
	lcd_rectangle_thickness(124, 23, 236, 43, 2, CUSTOM_2);
	lcd_drawRect(126, 25, 234, 41, CYAN);
	lcd_rectangle_thickness(127, 26, 233, 40, 2, CUSTOM_2);
	lcd_fillRect(129, 28, 231, 38, CYAN);
}

void lcd_brake_on() {
	// Brake circle - ON state.
	lcd_circle_thickness(200, 280, 35, 2, CUSTOM_2);
	lcd_circle(200, 280, 33, RED);
	lcd_circle_thickness(200, 280, 32, 2, CUSTOM_2);
	lcd_fillcircle(200, 280, 30, RED);
	// Brake label.
	lcd_putBrakeLabelString(175, 274, "BRAKE");
}

void lcd_dms_on() {
	// DMS circle - ON state.
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_circle(72, 289, 26, YELLOW);
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_fillcircle(72, 289, 19, YELLOW);
}

void lcd_max_vel_on() {
	// MAX VEL circle - ON state.
	lcd_circle_thickness(129, 289, 26, 3, CUSTOM_2);
	lcd_circle(129, 289, 23, YELLOW);
	lcd_circle_thickness(129, 289, 22, 3, CUSTOM_2);
	lcd_fillcircle(129, 289, 19, YELLOW);
}

void lcd_sd_on() {
	// Contains counters for SD symbol.
	int i = 0;
	int j = 0;
	// SD rectangle - ON state.
	lcd_rectangle_thickness(7, 264, 39, 314, 2, CUSTOM_2);
	lcd_drawRect(9, 266, 37, 312, WHITE);
	lcd_rectangle_thickness(10, 267, 36, 311, 2, CUSTOM_2);
	lcd_fillRect(12, 269, 34, 309, WHITE);
	// SD symbol.
	lcd_fillRect(16, 277, 30, 305, CUSTOM_2);
	while (i < 4) {
		lcd_fillRect(17 + i, 276 - i, 30, 276 - i, CUSTOM_2);
		i++;
	}
	while (j < 5) {
		lcd_fillRect(21 + 2*j, 274, 21 + 2*j, 276, YELLOW);
		j++;
	}
}

/////////////////////////////////////
///*    LCD RUN STATICS (OFF)    *///
/////////////////////////////////////

void lcd_velocity_off() {
	// Velocity circle - OFF state.
	lcd_circle_thickness(98, 144, 92, 2, CUSTOM_2);
	lcd_circle(98, 144, 90, LIGHT_GRAY);
	lcd_circle_thickness(98, 144, 89, 2, CUSTOM_2);
	lcd_fillcircle(98, 144, 87, LIGHT_GRAY);
	// Velocity label.
	lcd_putVelLabelString(62, 182, "KM/H");
}

void lcd_throttle_plus_off() {
	// Throttle (+) rectangle - OFF state.
	lcd_rectangle_thickness(200, 68, 234, 149, 2, CUSTOM_2);
	lcd_drawRect(202, 70, 232, 147, MAGENTA);
	lcd_rectangle_thickness(203, 71, 231, 146, 2, CUSTOM_2);
	lcd_fillRect(205, 73, 229, 144, MAGENTA);
	// Plus symbol.
	lcd_fillRect(216, 98, 218, 119, CUSTOM_2);
	lcd_fillRect(208, 107, 226, 110, CUSTOM_2);
}

void lcd_throttle_minus_off() {
	// Throttle (-) rectangle - OFF state.
	lcd_rectangle_thickness(200, 154, 234, 235, 2, CUSTOM_2);
	lcd_drawRect(202, 156, 232, 233, LIGHT_GRAY);
	lcd_rectangle_thickness(203, 157, 231, 232, 2, CUSTOM_2);
	lcd_fillRect(205, 159, 229, 230, LIGHT_GRAY);
	// Minus symbol.
	lcd_fillRect(208, 193, 226, 196, CUSTOM_2);
}

void lcd_stops_off() {
	// Current stop rectangle - OFF state.
	lcd_rectangle_thickness(5, 23, 117, 43, 2, CUSTOM_2);
	lcd_drawRect(7, 25, 115, 41, LIGHT_GRAY);
	lcd_rectangle_thickness(8, 26, 114, 40, 2, CUSTOM_2);
	lcd_fillRect(10, 28, 112, 38, LIGHT_GRAY);
	// Next stop rectangle - OFF state.
	lcd_rectangle_thickness(124, 23, 236, 43, 2, CUSTOM_2);
	lcd_drawRect(126, 25, 234, 41, LIGHT_GRAY);
	lcd_rectangle_thickness(127, 26, 233, 40, 2, CUSTOM_2);
	lcd_fillRect(129, 28, 231, 38, LIGHT_GRAY);
}

void lcd_brake_off() {
	// Brake circle - OFF state.
	lcd_circle_thickness(200, 280, 35, 2, CUSTOM_2);
	lcd_circle(200, 280, 33, LIGHT_GRAY);
	lcd_circle_thickness(200, 280, 32, 2, CUSTOM_2);
	lcd_fillcircle(200, 280, 30, LIGHT_GRAY);
	// Brake label.
	lcd_putBrakeLabelString(175, 274, "BRAKE");
}

void lcd_dms_off() {
	// DMS circle - OFF state.
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_circle(72, 289, 26, LIGHT_GRAY);
	lcd_circle_thickness(72, 289, 26, 3, CUSTOM_2);
	lcd_fillcircle(72, 289, 19, LIGHT_GRAY);
}

void lcd_max_vel_off() {
	// MAX VEL circle - OFF state.
	lcd_circle_thickness(129, 289, 26, 3, CUSTOM_2);
	lcd_circle(129, 289, 23, LIGHT_GRAY);
	lcd_circle_thickness(129, 289, 22, 3, CUSTOM_2);
	lcd_fillcircle(129, 289, 19, LIGHT_GRAY);
}

///////////////////////////////////
///*    LCD SHAPE THICKNESS    *///
///////////////////////////////////

// Draws rectangles of a specified thickness.
void lcd_rectangle_thickness(unsigned short x0, unsigned short y0, unsigned short x1,
							 unsigned short y1, int thickness, lcd_color_t color) {
	int i = 0;
	while (i < thickness) {
		lcd_drawRect(x0 + i, y0 + i, x1 - i, y1 - i, color);
		i++;
	}

}

// Draws circles of a specified thickness.
void lcd_circle_thickness(unsigned short x0, unsigned short y0, unsigned short r, 
						  int thickness, lcd_color_t color) {
	int i = 0;
	while (i < thickness) {
		lcd_circle(x0 , y0, r - i, color);
		i++;
	}

}

///////////////////////////////////////////
///*    INTEGER TO STRING CONVERTER    *///
///////////////////////////////////////////

// Reverses a string.
void reverse_string(unsigned char string[], int length) {
	int start = 0, end = length - 1;
	while (start < end) {
		char temp = *(string + start);
		*(string + start) = *(string + end);
		*(string + end) = temp;
		start++;
		end--;
	}
}

// Converts from an integer to a string in a specified radix.
unsigned char* integer_to_string(int integer, unsigned char* string, int radix) {
	int i = 0, neg_check = FALSE;
	if (integer == 0) {
		string[i + 1] = '0';
		string[i] = '\0';
		return string;
	}
	if (integer < 0 && radix == 10) {
		integer = -integer;
		neg_check = TRUE;
	}
	while (integer != 0) {
		int remainder = integer % radix;
		string[i + 1] = (remainder >= 10) ? (remainder - 10) + 'a' : remainder + '0';
		integer = integer / radix;
	}
	if (neg_check == TRUE) {
		string[i + 1] = '-';
	}
	string[i] = '\0';
	reverse_string(string, i);
	return string;
}
