// Main file for integration

#include <master_controller.h>

int main(void) {
    light_rail->brake_state = 0x1;
    light_rail->dms_state = 0x0;
    light_rail->vel_setpoint = 0;
    light_rail->velocity = 0;

    hw_init();

    start_controller();

    return 0;
}

void hw_init(void) {
    init_pll();                 // starts pll and sets cclk to 60mhz
    init_adc();
    init_dac();                 
    init_pwm();
}
