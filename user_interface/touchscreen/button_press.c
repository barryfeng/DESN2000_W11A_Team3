#include "button_press.h"

extern LightRail light_rail;

//Attempts to press a button
//Accepts an x and y co-ordinate ranging from 0-255
void press_button(char x, char y) {
	//scale the coordinates to the screens resolution
	int point_x = x*240/255;
	int point_y = y*320/255;

	//activate the brake if the distance is less than the radius
	if (inside_brake(point_x, point_y)) {
		//activate brake
		light_rail.brake_state = !light_rail.brake_state;
	} else if (inside_throttle_plus(point_x, point_y)) {
		//activate increase throttle
		if (light_rail.velocity < 180) {
			light_rail.velocity += ceil(5/3.6) << 16;
		}
	} else if (inside_throttle_minus(point_x, point_y)) {
		//activate decrease throttle
		if (light_rail.velocity > 0) {
			light_rail.velocity -= ceil(5/3.6) << 16;
		}
	} else if (inside_sd(point_x, point_y)) {
		//activate diagnostic code running
		light_rail.drive_state = !light_rail.drive_state;
	}

	//for debugging purposes draw dots where touches are registered
	//lcd_point(point_x, point_y, BLACK);
}

int inside_brake(int point_x, int point_y) {
	int distance = ((point_x - BRAKE_X)*(point_x - BRAKE_X) + 
					(point_y - BRAKE_Y)*(point_y - BRAKE_Y));

	if (distance < BRAKE_R*BRAKE_R) {
		return 1;
	}
	
	return 0;
}

int inside_throttle_plus(int point_x, int point_y) {
	if ((point_x >= THROTTLE_PLUS_X0) && (point_x <= THROTTLE_PLUS_X1)) {
		if ((point_y >= THROTTLE_PLUS_Y0) && (point_y <= THROTTLE_PLUS_Y1)) {
			return 1;
		}
	}

	return 0;
}

int inside_throttle_minus(int point_x, int point_y) {
	if ((point_x >= THROTTLE_MINUS_X0) && (point_x <= THROTTLE_MINUS_X1)) {
		if ((point_y >= THROTTLE_MINUS_Y0) && (point_y <= THROTTLE_MINUS_Y1)) {
			return 1;
		}
	}

	return 0;
}

int inside_sd(int point_x, int point_y) {
	if ((point_x >= SD_X0) && (point_x <= SD_X1)) {
		if ((point_y >= SD_Y0) && (point_y <= SD_Y1)) {
			return 1;
		}
	}

	return 0;
}

// //Draw a new bubble
// void newbubble() {
// 	// place and size a random bubble so it does not escape the screen
// 	bubble_r = 10 + (rand() % 30);
// 	bubble_x = (bubble_r*2) + (rand() % (240 - bubble_r*4));
// 	bubble_y = (bubble_r*2) + (rand() % (320 - bubble_r*4));
	
// 	// draw the bubble on the screen
// 	lcd_fillScreen(BLUE_BG);	
// 	lcd_fillcircle(bubble_x, bubble_y, bubble_r, PURPLE);
// }