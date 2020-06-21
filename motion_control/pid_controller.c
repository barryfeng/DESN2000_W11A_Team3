#include <pid_controller.h>
#include <./constants.h>

#include <LPC24XX.h>

#include "stdint.h"

#define CCLK_DELAY 6000

int get_correction(void) {
    const double time_cycle = 5;    // 5 ms

    double error = 0;
    double p_error = 0;
    
    double p_integral = 0;

    double v_setpoint = 0;

    get_error(&error, &p_error, &p_integral, v_setpoint, time_cycle);

    return 0;
}

void init_adc(void) {
    PCONP |= (1 << 12);             // Power on ADC Module
    AD0CR |= (1 << 21);             // Enable ADC
    PCLKSEL0 |= (0x01 << 24);
    PINSEL1 |= (0b01 << 14);        // Select P0.23 to AD0[0]
}

double get_vel(void) {
    double vel = 10;

    return vel;
}

double get_error(double *error, double *p_error, double *p_integral, double v_setpoint, double time_cycle) {
    *error = v_setpoint - get_vel();
    double integral = *p_integral * time_cycle;
    double derivative = (*error - *p_error) / time_cycle;

    double out = kP * (*error) + kI * (*error) + kD * (*error);

    *p_error = *error;
    *p_integral = integral;

    delay_ms(time_cycle);
}

void delay_ms(uint16_t j) {
    for (uint16_t i = 0; i < j; i++) {
        for (uint16_t x = 0; x < CCLK_DELAY; x++);    // 1 ms delay at 60MHz CCLK
    }
}