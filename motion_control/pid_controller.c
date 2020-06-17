/*
    GLOBALS: VELOCITY
*/

#include <stdio.h>

#define kP 10
#define kI 0
#define kD 0

int main(void) {
    double time_cycle = 0.05;

    double error = 0;
    double p_error = 0;
    
    double p_integral = 0;

    double v_setpoint = 0;

    get_error(&error, &p_error, &p_integral, v_setpoint, time_cycle);

    return 0;
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

    sleep(time_cycle);
}

