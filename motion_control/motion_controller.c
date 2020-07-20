/**
 * FILENAME :        motion_controller.c
 * DESCRIPTION :     PWM Generation for DESN2000 ELEC W11A Group 3 
 * 
 * NOTES :
 * 
 * AUTHOR :    Barry Feng   
 * 
 * START DATE :    20 Jun 20
 * 
 * CHANGES :
 * -- 20/06/2020 --     File created.
 */

#include <motion_controller.h>

static int get_setpoint(void) {
    return 10;
}

static double get_voltage(void) {
    static uint32_t result;

    // maybe put some sort of filter on this data lmao :(
    AD0CR = AD0CR | (1 << 24);  // start adc start
    while (!(AD0DR1 & 0x80000000));  // wait for adc to finish
    result = AD0DR1;
    result = (result >> 6);
    result = (result & 0x000003FF);
    
    return ((result / 1023.0) * 3.3);  // Convert ADC value to equivalent voltage
}

static void start_pwm(void) {
    // ! TODO: BARRY SETUP PWM0
    PWM0TCR |= 0x00000009;

    set_pwm(0);
}

static void set_pwm(int duty_cycle) {
    if (T1IR & 0x1) {
        PWM0MR1 = duty_cycle;   // Set new speed
        PWM0LER = (1 << 1);     // Latch to new speed
    }
}

void start_controller(void) {
    init_timer3('m');
    start_pwm();

    Controller pi_controller = init_controller(kP, kI);

    while (1) {
        set_pwm(step_controller(get_setpoint(), get_vel(1), pi_controller));
        delay_timer3(CYCLE_TIME);
    }
}

// Use arg 1 to return m/s, arg 0 to return km/h
double get_vel(int ret_type) {
    double voltage = get_voltage();
    double vel_kmh = voltage * (100 / 2.5);  // in kmh
    double vel_ms = vel_kmh / 3.6;           // in m/s
    double rpm = vel_ms / (WHEEL_OD / 2) * (60 / (2 * acos(-1)));   // w (rpm) = v/r

    return ret_type ? vel_ms : vel_kmh;
}