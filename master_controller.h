#ifndef MASTCNTRL

#include "motion_controller.h"
#include "system_init.h"
#include "stdbool.h"
#include "stdlib.h"

typedef struct {
    uint32_t velocity;
    uint32_t vel_setpoint;
    uint8_t brake_state;
    uint8_t dms_state;
} *LightRail;

LightRail light_rail;

#endif