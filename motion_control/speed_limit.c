/**
 * FILENAME :           speed_limit.c
 * DESCRIPTION :        Maximum permitted velocity checks for light_rail at
 *                      certain location.
 * 
 * NOTES :
 * 
 * AUTHOR :             Barry Feng   
 * 
 * START DATE :         2 Aug 20
 * 
 * CHANGES :
 * -- 02/08/2020 --     File created.
 */

#include "speed_limit.h"

uint8_t speed_limit_data[SPEED_LIM_DATA_SIZE];

/**
 * The following function initialises a 1D array containing the speed limit
 * data for the light rail network.
 * 
 * Each location ID is given a maximum permitted velocity. The location ID 
 * corresponds to the index of the array.
 */
void init_vel_limit_data(void) {
    speed_limit_data[0] = 40;
    speed_limit_data[1] = 50;
    speed_limit_data[2] = 50;
    speed_limit_data[3] = 20;
    speed_limit_data[4] = 30;
    speed_limit_data[5] = 10;
    speed_limit_data[6] = 40;
    speed_limit_data[7] = 5;
}

/**
 * This function returns the maximum permitted velocity at a certain location
 * given by the argument loc_id in Q22 notation (8 bit velocity shifted by
 * 16 bits).
 */
uint32_t get_vel_limit_data(uint8_t loc_id) {
    return (speed_limit_data[loc_id] << Q_SHIFT);
}
