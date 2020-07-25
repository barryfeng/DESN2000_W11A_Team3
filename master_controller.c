// Main file for integration

#include <master_controller.h>

LightRail light_rail;

int main(void) {
    Controller lr_controller;

    lr_init();
    hw_init();

    lr_controller = start_controller();
    // run this inside fiq interrupt

    init_timer0(CYCLE_TIME, 'm', run_controller, lr_controller);
}

void hw_init(void) {
    init_pll();                 // starts pll and sets cclk to 60mhz
    init_adc();
    init_dac();                 
    init_pwm();
}

void lr_init(void) {
    memset(&light_rail, '0', sizeof(LightRail));
    light_rail.velocity = 0x1;
    light_rail.dms_state = 0x0;
    light_rail.vel_setpoint = 0;
    light_rail.velocity = 0;
}
