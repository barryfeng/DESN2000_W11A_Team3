#include <pid_controller.h>
#include <./constants.h>

int get_correction(void) {
    const double time_cycle = 0.05;

    double error = 0;
    double p_error = 0;
    
    double p_integral = 0;

    double v_setpoint = 0;

    get_error(&error, &p_error, &p_integral, v_setpoint, time_cycle);

    return 0;
}

// Use ADC to convert 0-2.5V encoder signal to rpm (and m/s)
double get_vel(int unit) {
    double vel = 10;

    // unit == 1 is rpm, unit == 0 is m/s
    return unit ? vel : vel/100;
}

double get_error(double *error, double *p_error, double *p_integral, double v_setpoint, double time_cycle) {
    *error = v_setpoint - get_vel(1);
    double integral = *p_integral * time_cycle;
    double derivative = (*error - *p_error) / time_cycle;

    double out = kP * (*error) + kI * (*error) + kD * (*error);

    *p_error = *error;
    *p_integral = integral;

    sleep(time_cycle);
}

