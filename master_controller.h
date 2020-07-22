#ifndef MASTCNTRL

#include "motion_controller.h"
#include "system_init.h"
#include "stdbool.h"
#include "stdlib.h"

typedef struct lr {
    uint32_t velocity;
    uint32_t vel_setpoint;
    uint8_t brake_state;
    uint8_t dms_state;
} *LightRail;

struct lr lr_init = {0, 0, 0, 0};
LightRail light_rail = &lr_init;

#endif