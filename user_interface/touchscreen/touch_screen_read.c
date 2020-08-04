#include <touch_screen_read.h>

void touch_screen_press(void) {
	char x=0, y=0, z1=0, z2=0, pressure=0, threshold=100;
	
	//Read in X and Y coordinates
	touch_read_xy(&x,&y, &z1, &z2);
	//Calculate the pressure
	pressure_calc(&pressure, x, y, z1, z2);
	
	//press the button
	if (pressure > threshold) {
		press_button(x, y);
	}
}

void pressure_calc(char *pressure, char x, char y, char z1, char z2) {
	*pressure = 1000.0 * (x / 4096.0) * (z2 / z1 - 1.0);
}
