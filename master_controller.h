#ifndef MASTCNTRL

#include "motion_controller.h"
#include "system_init.h"
#include "stdbool.h"
#include "stdlib.h"

typedef struct lr {
    uint32_t velocity;          // Read from this.
    uint32_t vel_setpoint;      // Write to this.
    uint8_t brake_state;        // Write to this.
    uint8_t dms_state;          // Read from this.
} *LightRail;

struct lr lr_init = {0, 0, 0, 0};
LightRail light_rail = &lr_init;

#endif