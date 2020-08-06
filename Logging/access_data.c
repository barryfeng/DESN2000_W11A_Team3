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

#include "access_data.h"

/* 
    This struct contains the light rail data:
    - velocity: 32 bits
    - target velocity: 32 bits
    - brake state: 8 bits
    - DMS state: 8 bits
*/
extern LightRail light_rail;

extern void store(uint32_t vel, uint32_t vel_setpoint, uint8_t brake, uint8_t dms);
extern uint32_t view_vel(void);
extern uint32_t view_vel_setpoint(void);
extern uint8_t view_brake(void);
extern uint8_t view_dms(void);

/*
    store_data is called to store the 4 pieces of data into memory.
    The data is acquired from the light_rail struct. The store function
    is called from the data_management assembly file. The data is passed
    to this function as arguments.
*/

void store_data() {
    uint32_t velocity = light_rail.velocity;
    uint32_t vel_setpoint = light_rail.vel_setpoint;
    uint8_t brake_state = light_rail.brake_state;
    uint8_t dms_state = light_rail.dms_state;

    store(velocity, vel_setpoint, brake_state, dms_state);
}

/*
    The access_velocity function returns the velcoity of the light rail
    as a 32 bit number. The view_vel function is called from the data_management
    assembly file which will return the velocity data from memory.
*/

uint32_t access_velocity() {
    uint32_t velocity = view_vel();
    return velocity;
}

/*
    The access_velocity_setpoint function returns the target velcoity of 
    the light rail as a 32 bit number. The view_vel_setpoint function is called
    from the data_management assembly file which will return the velocity set point
    data from memory.
*/

uint32_t access_velocity_setpoint() {
    uint32_t velocity_setpoint = view_vel_setpoint();
    return velocity_setpoint;
}

/*
    The access_brake_state function returns the brake state of the light rail
    as an 8 bit number. 1 will be returned if the brake is active and 0 if it
    is not. The view_brake function is called from the data_management
    assembly file which will return the brake data from memory.
*/

uint8_t access_brake_state() {
    uint32_t brake_state = view_brake();
    return brake_state;
}

/*
    The access_brake_state function returns the DMS (dead man switch) state
    of the light rail as an 8 bit number. 1 will be returned if the DMS is
    active and 0 if it is not. The view_dms function is called from the
    data_management assembly file which will return the dms data from memory.
*/

uint8_t access_dms_state() {
    uint32_t dms_state = view_dms();
    return dms_state;
}
