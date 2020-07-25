// Main file for integration

#include <master_controller.h>

LightRail light_rail;

int main(void) {
    lr_init();
    hw_init();
    
    start_master_isr(CYCLE_TIME, 'm');
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
