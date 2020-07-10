// Main file for integration
#include "system_init.h"
#include "motion_controller.h"

double velocity;
double brake_state; 
double dms_state;


int main(void) {
    start_controller();

    return 0;
}

void hw_init(void) {
    init_pll();                 // starts pll and sets cclk to 60mhz
    init_adc();                 
    init_pwm(PWM_DC_DEF);
    init_uart();
}