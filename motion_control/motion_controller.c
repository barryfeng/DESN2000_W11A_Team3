/**
 * FILENAME :           motion_controller.c
 * DESCRIPTION :        PWM Generation for DESN2000 ELEC W11A Group 3 
 * 
 * NOTES :
 * 
 * AUTHOR :             Barry Feng   
 * 
 * START DATE :         20 Jun 20
 * 
 * CHANGES :
 * -- 20/06/2020 --     File created.
 */

#include <motion_controller.h>

extern LightRail light_rail;

static uint32_t get_voltage(void) {

    static uint16_t result;

    AD0CR = AD0CR | (1 << 24);  // start adc start
    while (!(AD0DR1 & 0x80000000));  // wait for adc to finish
    result = AD0DR1;
    result = (result >> 6);
    result = (result & 0x000003FF);

    // Convert ADC value to equivalent voltage (result*3.3/1023 << 16)
    return ((0x34DA * result) >> (ADC_SHIFT - VEL_SHIFT));
}

static void start_pwm(void) {
    PWM0LER = (1 << 0) | (1 << 1);      // Update PWM0 Latch for MR0, MR1
    PWM0TCR = (1 << 0) | (1 << 3);      // Enable PWM0 and Reset PWM0 TC
}

static void set_pwm(int duty_cycle) {
    PWM0MR1 = duty_cycle;               // Set new PWM0 MR1 match value
    PWM0LER = (1 << 1);                 // Update PWM0 Latch for MR0, MR1
}

Controller start_controller(void) {
    Controller pi_controller = init_controller(kP, kI);
    start_pwm();

    return pi_controller;
}

void run_controller(Controller pi_controller) {
    static uint32_t compensation = 0;
    static uint8_t dms_state = 0, mem_dms_state = 0;

    update_dms_state(dms_state, mem_dms_state);
    update_brake_state();

    compensation = step_controller(
        light_rail.vel_setpoint,
        light_rail.velocity = get_vel(),
        pi_controller);

    if (!(light_rail.brake_state)) {
        set_pwm(abs(compensation));
    }
}

/**
 * This function updates the light rail's DMS state by reading from GPIO
 * pin P0.10 on the FVGA daughter board. The function uses edge-triggering
 * to detect changes in the DMS. If a LOW->HIGH change is detected, the
 * DMS system is reset (control is given to the driver). If a HIGH->LOW
 * change is detected, the DMS system is set (control is taken from the
 * driver and brakes are set).
 */
static void update_dms_state(uint8_t dms_state, uint8_t mem_dms_state) {
    // Read from DMS P0.10
    dms_state = ((FIO0PIN >> 10) & 0x1);

    if (dms_state != mem_dms_state) {
        if (dms_state == 1) {
            reset_dms();
        } else {
            set_dms();
        }
        mem_dms_state = dms_state;
    }
}

/**
 * This function updates the light rail's brakes based on the light_rail
 * struct's brake_state value.
 */
static void update_brake_state(void) {
    if (light_rail.brake_state) {
        set_brake();
		return;
    }
    release_brake();
}

/**
 * This function actiates the light rail's brakes by driving the P1.0 GPIO
 * pin HIGH. The light rail's velocity setpoint is also set to 0. 
 */
static void set_brake(void) {
    light_rail.vel_setpoint = 0;
    FIO1PIN |= (1 << 0);
}

/**
 * This function releases the light rail's brakes by driving the P1.0 GPIO
 * pin LOW.
 */
static void release_brake(void) {
    FIO1PIN &= ~(1 << 0);
}

/**
 * This function sets the DMS system. If called, the brakes are activated
 * and the light rail's velocity setpoint is set to 0.
 */
static void set_dms(void) {
    set_brake();
    light_rail.dms_state = 0x1;
}

/**
 * This function resets the DMS system. If called, the brakes are released
 * and normal light rail operation can continue.
 */
static void reset_dms(void) {
    release_brake();
    light_rail.dms_state = 0x0;
}

/**
 * This function returns a binary fixed-point velocity which is Q16.0.
 * The returned result is also right shifted by 16 bits from the true velocity
 * to maintain resolution. The velocity in meters per second is obtained by 
 * adjusting the voltage by a scaling factor of (100/(2.5 * 3.6) = 11.11). This
 * is approximated to 11 to return an integral result.
 */ 
static uint32_t get_vel(void) {
    uint32_t voltage = get_voltage();
    uint32_t vel_ms = voltage * 11;

    return vel_ms;
}
