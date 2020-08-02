#ifndef __ACCESS_DATA__
#define __ACCESS_DATA__

#include "master_controller.h"
extern LightRail light_rail;

extern void store(uint32_t vel, uint32_t vel_setpoint, uint8_t brake, uint8_t dms);
extern uint32_t view_vel();
extern uint32_t view_vel_setpoint();
extern uint8_t view_brake();
extern uint8_t view_dms();

void store_data();
uint32_t access_velocity();
uint32_t access_velocity_setpoint();
uint8_t access_brake_state();
uint8_t access_dms_state();

#endif