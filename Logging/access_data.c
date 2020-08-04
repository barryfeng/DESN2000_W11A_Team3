/**
 * FILENAME :        access_data.c
 * DESCRIPTION :     API for accessing and storing data
 * SUPERSEEDED:      OLD_access_data.c
 * 
 * NOTES :           use this file to store or view logged data
 * 
 * AUTHOR :          Adam Witt   
 * 
 * START DATE :      1 Aug 20
 * 
 * CHANGES :
 * -- 1/08/2020 --  File created.
 */

#include "access_data.h";

extern LightRail light_rail;

extern void store(uint32_t vel, uint32_t vel_setpoint, uint8_t brake, uint8_t dms);
extern uint32_t view_vel(void);
extern uint32_t view_vel_setpoint(void);
extern uint8_t view_brake(void);
extern uint8_t view_dms(void);

void store_data() {
    uint32_t velocity = light_rail.velocity;
    uint32_t vel_setpoint = light_rail.vel_setpoint;
    uint8_t brake_state = light_rail.brake_state;
    uint8_t dms_state = light_rail.dms_state;

    store(velocity, vel_setpoint, brake_state, dms_state);
}

uint32_t access_velocity() {
    uint32_t velocity = view_vel();
    return velocity;
}

uint32_t access_velocity_setpoint() {
    uint32_t velocity_setpoint = view_vel_setpoint();
    return velocity_setpoint;
}

uint8_t access_brake_state() {
    uint32_t brake_state = view_brake();
    return brake_state;
}

uint8_t access_dms_state() {
    uint32_t dms_state = view_dms();
    return dms_state;
}
