// Main file for integration
#include <master_controller.h>

int main(void) {
    hw_init();
    light_rail = lr_init();

    start_controller();

    return 0;
}

void hw_init(void) {
    init_pll();                 // starts pll and sets cclk to 60mhz
    init_adc();
    init_dac();                 
    init_pwm();
    init_uart();
}

LightRail lr_init(void) {
    light_rail->brake_state = true;
    light_rail->dms_state = false;
    light_rail->velocity = 0.0f;

    return light_rail;
}