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

// State names for ISR states
#define ISR_ACTIVE 1
#define ISR_INACTIVE 0

// Light rail structure initialisation values
#define SYS_START_BRAKE_STATE 1
#define SYS_START_DMS_STATE 0
#define SYS_START_VEL_SETPOINT 0
#define SYS_START_VELOCITY 0
#define SYS_START_DRIVE_STATE 1
#define SYS_START_MASTER_TMR_STATE 0

// System ISR cycle time (in ms)
#define CYCLE_TIME 20

void hw_init(void);
void lr_init(void);

/** 
 * Light rail central structure. This contains all important parameters and
 * data pertaining to light rail operation.
 */
typedef struct lr {
    uint32_t velocity;
    uint32_t vel_setpoint;
    uint8_t brake_state;
    uint8_t dms_state;
    uint8_t drive_state;
    uint8_t master_tmr_state;   
} LightRail;


#endif
