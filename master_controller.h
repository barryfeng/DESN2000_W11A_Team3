#ifndef MASTCNTRL_H

#define MASTCNTRL_H

//Size of logged data (aligned to 4 words)
#define DATA_LOG_SIZE 160 

#include "stdint.h"
#include "string.h"

#include "motion_controller.h"
#include "./system/system_init.h"
#include "diagnostics/diagnostics.h"
#include "lcd_main.h"

#define ISR_ACTIVE 1
#define ISR_INACTIVE 0

void hw_init(void);
void lr_init(void);
typedef struct lr {
    uint32_t velocity;          // Read from this.
    uint32_t vel_setpoint;      // Write to this.
    uint8_t brake_state;        // Write to this.
    uint8_t dms_state;          // Read from this.
    uint8_t drive_state;        // Read from this.
    uint8_t master_tmr_state;   
} LightRail;


#endif
