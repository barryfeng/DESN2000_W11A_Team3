/*
#include <./constants.h>

int main(void) {
    double max_vel_time = 0;
    double max_vel_cond = 0;
    double max_vel_pop_dens = 0;
    
    get_max_vel(&max_vel_time, &max_vel_cond, &max_vel_pop_dens);

    // Code to handle with all velocities below max velocity controlled by the lever.

    return 0;
}

double get_max_vel(double *max_vel_time, double *max_vel_cond, double *max_vel_pop_dens) {
    
    double max_vel = 0;

    if (max_vel_time <= max_vel_cond && max_vel_time <= max_vel_pop_dens) {
        max_vel = max_vel_time;
    } else if (max_vel_cond <= max_vel_time && max_vel_cond <= max_vel_pop_dens) {
        max_vel = max_vel_cond;
    } else {
        max_vel = max_vel_pop_dens;
    }

    return max_vel;
}
*/

