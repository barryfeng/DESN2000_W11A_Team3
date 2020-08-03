#ifndef __ACCESS_DATA__
#define __ACCESS_DATA__

#include "master_controller.h"

void store_data(void);
uint32_t access_velocity(void);
uint32_t access_velocity_setpoint(void);
uint8_t access_brake_state(void);
uint8_t access_dms_state(void);

#endif
