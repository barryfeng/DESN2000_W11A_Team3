/**
 * FILENAME :        access_data.c
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

void store_data() {
    uint32_t velocity = light_rail.velocity;
    uint32_t vel_setpoint = light_rail.vel_setpoint;
    uint8_t brake_state = light_rail.brake_state;
    uint8_t dms_state = light_rail.dms_state;

    //String that contains all the data from the function arguments
    char data_string[64] = "";  //Size 64 is sufficient and also allows data to be aligned every two words

    char *velocity;
    sprintf(velocity, "Velocity = %d km/h. ", velocity);

    char *brake;
    sprintf(brake, "Brake state = %d. /n", brake);

    strcat(data_string, velocity); //Concatenate strings
    strcat(data_string, brake); //Concatenate strings

    //Call function in assembly to store the data
    data_start(data_string, 0);
}