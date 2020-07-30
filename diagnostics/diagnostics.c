#include <diagnostics.h>

/**
 * This function takes in the address of the code loaded into SDRAM from the 
 * SD card and executes the code at that address.
 */
void run_diag_code(unsigned int *code) {
    void (*func_ptr)(void) = &code;
    (*func_ptr)();
}

int check_diag_conditions(void) {
    if (light_rail.drive_state == DRIVE_ACTIVE) {
        return 0;
    } else if (light_rail.velocity != 0) {
        return 0;
    } else if (light_rail.vel_setpoint != 0) {
        return 0;
    } else if (light_rail.brake_state == BRAKE_INACTIVE) {
        return 0;
    }

    return 1;
}