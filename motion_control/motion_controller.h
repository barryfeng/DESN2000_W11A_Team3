#ifndef MOTION_CONTROLLER_H

#include <LPC24XX.h>
#include <stdint.h>

#define CCLK_DELAY 6000
#define WHEEL_OD 0.3  // in meters
#define kP 10
#define kI 0
#define kD 0

static int get_setpoint(void);
static double get_voltage(void);
static void start_pwm(void);
static void set_pwm(int duty_cycle);
static void get_error(double *compensate, double *error, double *p_error, double *p_integral, double v_setpoint, double time_cycle);
static void delay_ms(uint32_t j);

void start_controller(void);

double get_vel(int ret_type);

#endif