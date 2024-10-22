/**
 * FILENAME :           master_controller.c
 * DESCRIPTION :        Master light rail integration file for DESN2000 
 *                      ELEC W11A Group 3 
 * 
 * NOTES :
 * 
 * AUTHOR :             Barry Feng   
 * 
 * START DATE :         24 Jun 20
 * 
 * CHANGES :
 * -- 24/06/2020 --     File created.
 */

#include <master_controller.h>

LightRail light_rail;

/**
 * This function initilises the light_rail structure which contains
 * the light rail's state and velocity data. The peripherals which
 * are required for light rail operation are also initialised.
 * 
 * An infinite loop is used to toggle the ISR interrupt system which
 * handles the PI controller, driver LCD console and system logging.
 * 
 * Information on the LightRail structure can be found in the
 * initialisation section (lr_init).
 * 
 * The system diagnostic mode can be started if the system drive_state 
 * set to 0 by the touch screen interface.
 */
int main(void) {
	uint16_t diagnostic_code[MAX_DIAGNOSTIC_CODE_SIZE];
	
    lr_init();
    hw_init();

    while (1) {
        if (light_rail.drive_state == DRIVE_ACTIVE) {
            if (light_rail.master_tmr_state == ISR_INACTIVE) {
                start_master_isr(CYCLE_TIME, 'm');
            }
        } else {
            if (check_diag_conditions()) {
                stop_master_isr();
				load_diag_code(diagnostic_code);
                run_diag_code(diagnostic_code);
            }
        }
    }

}

/**
 * This function configures the peripherals and system clock for the light
 * rail. The PLL configures the system clock to 60MHz. Additionally, the ADC,
 * PWM and SPI are initilised.
 */
void hw_init(void) {
    init_pll();
    init_adc();
    init_pwm();
    init_spi();
    init_lcd();
    init_ultrasonic();
    init_vel_limit_data();
    touch_init();
}

/**
 * This function initialises the components within the light rail's central
 * struct to predefined values on system startup.
 */
void lr_init(void) {
    memset(&light_rail, '0', sizeof(LightRail));
    light_rail.brake_state = SYS_START_BRAKE_STATE;
    light_rail.dms_state = SYS_START_DMS_STATE; 
    light_rail.vel_setpoint = SYS_START_VEL_SETPOINT;
    light_rail.velocity = SYS_START_VELOCITY;
    light_rail.drive_state = SYS_START_DRIVE_STATE;
    light_rail.master_tmr_state = SYS_START_MASTER_TMR_STATE;
}
