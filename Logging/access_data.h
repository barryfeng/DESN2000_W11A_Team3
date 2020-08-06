#ifndef __ACCESS_DATA__
#define __ACCESS_DATA__

#include "master_controller.h"

/*
    The following functions are used to store data and to access
    individual data items from memory. The logged data is velocity;
    target velocity (set point); brake state; and DMS state
    (dead man switch).
*/

void store_data(void);
uint32_t access_velocity(void);
uint32_t access_velocity_setpoint(void);
uint8_t access_brake_state(void);
uint8_t access_dms_state(void);

#endif
