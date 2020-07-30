#ifndef MOTION_CONTROLLER_H

#define MOTION_CONTROLLER_H

#include <LPC24XX.h>
#include <stdint.h>
#include <stdlib.h>

#include "master_controller.h"
#include "../system/system_timer.h"
#include "fixed_point_pid.h"
#include "../system/system_init.h"

#define kP 10.0
#define kI 0.0

#define VEL_SHIFT 16
#define ADC_SHIFT 22

#define CYCLE_TIME 5

#define BRAKE_ACTIVE 1
#define BRAKE_INACTIVE 0

#define DRIVE_ACTIVE 1
#define DRIVE_INACTIVE 0

static uint32_t get_voltage(void);

static void set_pwm(int);

void run_controller(void);

static void update_dms_state(uint8_t, uint8_t);
static void update_brake_state(void);

static void set_brake(void);
static void release_brake(void);
static void set_dms(void);
static void reset_dms(void);

uint32_t get_vel(void);

#endif
