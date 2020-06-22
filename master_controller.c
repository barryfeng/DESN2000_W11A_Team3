// Main file for integration
#include "system_init.h"

double velocity;
double brake_state;
double dms_state;


int main(void) {
    init_pll();                 // starts pll and sets cclk to 60mhz
    init_adc();                 
    init_pwm(PWM_DC_DEF);

    return 0;
}