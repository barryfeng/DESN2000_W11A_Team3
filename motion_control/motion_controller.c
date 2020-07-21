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
#include <master_controller.h>

static uint32_t get_setpoint(void) {
    return (light_rail->vel_setpoint << VEL_SHIFT);
}

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

// Max 10 bit value
void set_dac(int16_t out) {
    if (out > 0x3FF) {
        return;
    }

    DACR &= ~(0x3FF << 6);
    DACR |= (out << 6);
}

void start_controller(void) {
    init_timer3('m');
    start_pwm();

    Controller pi_controller = init_controller(kP, kI);

    uint32_t curr_vel = get_vel();
    uint32_t compensation = 0;

    while (1) {
        curr_vel = get_vel();
        light_rail->velocity = curr_vel;

        compensation = step_controller(get_setpoint(), curr_vel, pi_controller);

        if (!(light_rail->brake_state)) {
            set_pwm(abs(compensation));
        }
        // check dms state here
        delay_timer3(CYCLE_TIME);
    }
}

void set_brake(void) {
    // single bit button function
    light_rail->vel_setpoint = 0;
    light_rail->brake_state = 0x1;

    //! TODO: BARRY
    //! Set GPIO to brake
}

void reset_brake(void) {
    light_rail->brake_state = 0x0;
}

void set_dms(void) {
    light_rail->dms_state = 0x1;
}

void reset_dms(void) {
    light_rail->dms_state = 0x0;
}

/**
 * This function returns a binary fixed-point velocity which is Q16.0.
 * The returned result is also right shifted by 16 bits from the true velocity
 * to maintain resolution. The velocity in meters per second is obtained by 
 * adjusting the voltage by a scaling factor of (100/(2.5 * 3.6) = 11.11). This
 * is approximated to 11 to return an integral result.
 */ 
uint32_t get_vel(void) {
    uint32_t voltage = get_voltage();
    uint32_t vel_ms = voltage * 11;           

    return vel_ms;
}