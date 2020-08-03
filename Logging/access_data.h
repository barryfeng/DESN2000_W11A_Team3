#ifndef __ACCESS_DATA__
#define __ACCESS_DATA__

#include "master_controller.h"

extern void store(uint32_t vel, uint32_t vel_setpoint, uint8_t brake, uint8_t dms);
extern uint32_t view_vel(void);
extern uint32_t view_vel_setpoint(void);
extern uint8_t view_brake(void);
extern uint8_t view_dms(void);

void store_data(void);
uint32_t access_velocity(void);
uint32_t access_velocity_setpoint(void);
uint8_t access_brake_state(void);
uint8_t access_dms_state(void);

#endif
