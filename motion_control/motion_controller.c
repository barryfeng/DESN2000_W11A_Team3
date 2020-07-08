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
                                        // ? NOTE FOR BARRY: we use timer3 to prevent external memory startup conflicts
    T3TCR = 0x02;                       // Reset timer3
    T3IR |= 0x01;                       // Clear interrupt register
    T3MR0 = 10;                         // PWM period matching
    T3MCR = (1 << 0) | (1 << 1);        // Reset counter and set interrupt on match
    T3TCR = (1 << 0);                   // Start timer

    PWM0TCR |= 0x00000009;

    set_pwm(0);
}

static void set_pwm(int duty_cycle) {
    if (T1IR & 0x1) {
        PWM0MR1 = duty_cycle;   // Set new speed
        PWM0LER = (1 << 1);     // Latch to new speed
    }
}

static void get_error(double *compensate, double *error, double *p_error, double *p_integral, double v_setpoint, double time_cycle) {
    *error = v_setpoint - get_vel(1);
    double integral = *p_integral * time_cycle;
    double derivative = (*error - *p_error) / time_cycle;

    *compensate = kP * (*error) + kI * (*error) + kD * (*error);

    *p_error = *error;
    *p_integral = integral;

    delay_ms(time_cycle);
}

static void delay_ms(uint32_t j) {
    for (uint32_t i = 0; i < j; i++) {
        for (uint32_t x = 0; x < CCLK_DELAY; x++);  // 1 ms delay at 60MHz CCLK
    }
}

void start_controller(void) {
    const int time_cycle = 5;
    double error = 0, p_error = 0, p_integral = 0;
    start_pwm();

    double compensate = 0;

    while (1) {
        get_error(&compensate, &error, &p_error, &p_integral, get_setpoint(), time_cycle);
        set_pwm(compensate);
    }
}

// Use arg 1 to return m/s, arg 0 to return kmh
double get_vel(int ret_type) {
    double voltage = get_voltage();
    double vel_kmh = voltage * (100 / 2.5);  // in kmh
    double vel_ms = vel_kmh / 3.6;           // in m/s
    // double rpm = vel_ms / (WHEEL_OD / 2) * (60 / (2 * acos(-1)));   // w (rpm) = v/r

    return ret_type ? vel_ms : vel_kmh;
}