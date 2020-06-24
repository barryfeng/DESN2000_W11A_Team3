#include <./constants.h>
#include <LPC24XX.h>
#include <pid_controller.h>

#include "stdint.h"
#include "math.h"

#define CCLK_DELAY 6000
#define WHEEL_OD 0.3    // in meters

int start_pid(void) {
    const int time_cycle = 5;

    double error = 0;
    double p_error = 0;
    
    double p_integral = 0;

    double v_setpoint = 0;

    get_error(&error, &p_error, &p_integral, v_setpoint, time_cycle);

    return 0;
}

double get_vel(int ret_type) {  // TODO: Barry
    double vel_kmh = 10;                                            // in kmh
    // double vel_ms = vel_kmh / 3.6;                                  // in m/s
    // double rpm = vel_ms / (WHEEL_OD / 2) * (60 / (2 * acos(-1)));   // w (rpm) = v/r

    return vel_kmh;
}

double get_error(double *error, double *p_error, double *p_integral, double v_setpoint, double time_cycle) {
    *error = v_setpoint - get_vel(1);
    double integral = *p_integral * time_cycle;
    double derivative = (*error - *p_error) / time_cycle;

    double out = kP * (*error) + kI * (*error) + kD * (*error);

    *p_error = *error;
    *p_integral = integral;

    delay_ms(time_cycle);
}

void delay_ms(uint32_t j) {
    for (uint32_t i = 0; i < j; i++) {
        for (uint32_t x = 0; x < CCLK_DELAY; x++);  // 1 ms delay at 60MHz CCLK
    }
}