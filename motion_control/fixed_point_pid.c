/**
 * FILENAME :           fixed_point_pid.c
 * DESCRIPTION :        Fixed point PI controller for DESN2000 ELEC W11A Group 3 
 * 
 * NOTES :
 * 
 * AUTHOR :             Barry Feng   
 * 
 * START DATE :         20 Jul 20
 * 
 * CHANGES :
 * -- 20/07/2020 --     File created.
 */

#include "fixed_point_pid.h"

/**
 * This function initialises the PI controller and takes in two floating
 * point parameters kP and kI. These controller parameters are shifted to
 * 14-bit integers in Q14 notation. If the parameters exceed INT14_MAX,
 * their values are set to INT14_MAX. The same applies if the parameters
 * underflow INT14_MIN.
 * 
 * The controller output limits are also set to 16 bits. The controller 
 * integral sum is started at 0 and the initialised bit in controller
 * is set to 1 after this process is complete. The controller structure
 * is finally returned.
 */
Controller init_controller(float kP, float kI) {
    Controller pi_controller; 
    int16_t int14_max = ((1 << 14) - 1);
    int16_t int14_min = -(1 << 14);

    memset(&pi_controller, '0', sizeof(Controller));

    // Set PI parameters
    pi_controller.kP = f_to_q(kP);
    pi_controller.kI = f_to_q(kI);

    // Limit normalised PI gains to int14

    if (pi_controller.kP > int14_max) {
        pi_controller.kP = int14_max;
    } else if (pi_controller.kP < int14_min) {
        pi_controller.kP = int14_min;
    } else if (pi_controller.kI > int14_max) {
        pi_controller.kI = int14_max;
    } else if (pi_controller.kI < int14_min) {
        pi_controller.kI = int14_min;
    }

    // Set 16-bit signed output to normalised limits
    pi_controller.out_max = (0xFFFFUL >> 1) * PARAM_MULT;
    pi_controller.out_min = (int32_t) (-((0xFFFFUL + 1) >> 1) * PARAM_MULT);

    // Set integral sum to zero
    pi_controller.sum = 0;

    pi_controller.initialised = 1;

    return pi_controller;
}

/**
 * This function follows a standard integration of a proportional integral
 * controller. The error is obtained by taking the difference between
 * the velocity and velocity setpoint of the light rail. The proportional
 * and integral terms are evaluated and summed to form the controller
 * output. 
 * 
 * The controller output is then checked for saturation and 
 * overflow. If saturation or overflow occur, the output is capped at
 * the 16-bit out_max value specified in controller initialisation.
 */
int16_t step_controller(int16_t sp, int16_t fb, Controller pi_controller) {
    // int16 + int16 = int17
    int32_t err = sp - fb;
    int32_t P = 0, I = 0, out = 0;
    int16_t compensate = 0;

    if (pi_controller.kP) {
        // int14 (kP) * int17 (err) = int31 (P)
        P = pi_controller.kP * err;
    }

    if (pi_controller.kI) {
        // int14 (kI) * int17 (err) = int31 (sum)
        pi_controller.sum += pi_controller.kI * err;

        // Limit sum to 32-bit signed value so that it saturates, never overflows.
        if (pi_controller.sum > INTEG_MAX)
            pi_controller.sum = INTEG_MAX;
        else if (pi_controller.sum < INTEG_MIN)
            pi_controller.sum = INTEG_MIN;

        // int31 (I) = int31 (sum)
        I = pi_controller.sum;
    }

    // int31 (P) + int31 (I) = int32 (out)
    out = P + I;

    // Check controller output saturation
    if (out > pi_controller.out_max) {
        out = pi_controller.out_max;
    } else if (out < pi_controller.out_min) {
        out = pi_controller.out_min;
    }

    // Denormalise compensation value
    compensate = out >> PARAM_SHIFT;

    // Round denormalised value.
    if (out & (0x1 << (PARAM_SHIFT - 1))) {
        compensate++;
    }

    return compensate;
}

/**
 * This function converts a floating point number to Q notation.
 */
static uint32_t f_to_q(float in) {
    uint32_t qval;

    if (in > PARAM_MAX || in < 0) {
        return 0;
    }

    qval = in * PARAM_MULT;

    if (in != 0 && qval == 0) {
        return 0;
    }

    return qval;
}
