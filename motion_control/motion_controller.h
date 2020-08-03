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

#define CYCLE_TIME 20

#define BRAKE_ACTIVE 1
#define BRAKE_INACTIVE 0

#define DRIVE_ACTIVE 1
#define DRIVE_INACTIVE 0

#define ULTRAS_TRIGGER 0x0
#define ULTRAS_1_ECHO 0x1
#define ULTRAS_2_ECHO 0x2
#define ULTRAS_3_ECHO 0x3
#define ULTRAS_ALL 0xF
#define ULTRAS_SETUP_TIME 2
#define ULTRAS_TRIG_TIME 10
#define DIVIDE_3 0x55555556
#define Q_SHIFT 16

/**
 * Set minimum obsctacle distance at 4m for assistive-braking. The defined
 * value is obtained using:
 *      (4 * 2 / 0.0343) << 16
 * The multiply 2, divide 0.0343 scales the 4 meters for the speed of sound
 * and for the distance to the obstacle (and not back).
 */
#define MIN_OBSTACLE_DIST 0xE93C74

static uint32_t get_voltage(void);
static uint32_t get_ultrasonic_data(void);

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
