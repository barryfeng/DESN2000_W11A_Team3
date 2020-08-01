/**
 * FILENAME :        OLD_access_data.c
 * DESCRIPTION :     API for data_management.s
 * 
 * NOTES :           use this file to store or view logged data
 * 
 * AUTHOR :          Adam Witt   
 * 
 * START DATE :      26 Jul 20
 * 
 * CHANGES :
 * -- 12/07/2020 --  File created.
 */

#include "master_controller.h"

extern LightRail light_rail;

extern void data_start(char *data_string, int SWI_operation);

void store_data () {
    uint32_t velocity = light_rail.velocity;
    uint32_t vel_setpoint = light_rail.vel_setpoint;
    uint8_t brake_state = light_rail.brake_state;
    uint8_t dms_state = light_rail.dms_state;

    //String that contains all the data from the function arguments
    char data_out[DATA_LOG_SIZE] = "";
    
    //Add velocity to data_string
    char *vel_string = "";
    sprintf(vel_string, "Velocity = %d km/h. ", velocity);
    strcat(data_out, vel_string);

    //Add velocity set point to data_string
    char *vel_set_string = "";
    sprintf(vel_set_string, "Velocity Set Point = %d km/h. ", vel_setpoint);
    strcat(data_out, vel_set_string);

    //Add brake state to data_string
    char *brake_string = "";
    sprintf(brake_string, "Brake state = %d km/h. ", brake_state);
    strcat(data_out, brake_string);

    //Add dms_state to data_string
    char *dms_string = "";
    sprintf(dms_string, "DMS State = %d km/h.\n", dms_state);
    strcat(data_out, dms_string);

    //Call function in assembly to store the data
    data_start(data_out, 0);
}

LightRail view_current_data() {
    LightRail data;

    return data;
}