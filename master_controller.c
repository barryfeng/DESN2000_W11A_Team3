// Main file for integration

#include <master_controller.h>

LightRail light_rail;

int main(void) {
    lr_init();
    hw_init();
    

    while (1) {
        if (light_rail.drive_state == DRIVE_ACTIVE) {
            start_master_isr(CYCLE_TIME, 'm');
        } else {
            // update_drive_state()
            // i.e. edit lcd touch screen code to invert the drive state
            // i.e. lr.drive_state = !lr.drive_state
            if (check_diag_conditions()) {
                // load code here
            }
        }
    }

}

void toggle_master_isr(void) {
    if (light_rail.master_tmr_state == ISR_ACTIVE) {
        light_rail.master_tmr_state = ISR_INACTIVE;
    } else {
        light_rail.master_tmr_state = ISR_ACTIVE;
    }
}

void hw_init(void) {
    init_pll();
    init_adc();
    init_pwm();
    init_spi();
}

void lr_init(void) {
    memset(&light_rail, '0', sizeof(LightRail));
    light_rail.velocity = 0x1;
    light_rail.dms_state = 0x0; 
    light_rail.vel_setpoint = 0;
    light_rail.velocity = 0;
    light_rail.drive_state = 1;
    light_rail.master_tmr_state = 0;
}
