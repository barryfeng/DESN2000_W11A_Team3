#ifndef MASTCNTRL_H

#define MASTCNTRL_H

#include "stdint.h"
#include "string.h"

#include "motion_controller.h"
#include "./system/system_init.h"

void hw_init(void);

typedef struct lr {
    uint32_t velocity;          // Read from this.
    uint32_t vel_setpoint;      // Write to this.
    uint8_t brake_state;        // Write to this.
    uint8_t dms_state;          // Read from this.
} LightRail;


#endif
