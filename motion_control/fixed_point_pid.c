#include "fixed_point_pid.h"

Controller init_controller(float kP, float kI) {
    Controller pi_controller = malloc(sizeof(Controller));

    // Set PI parameters
    pi_controller->kP = normalise(kP);
    pi_controller->kI = normalise(kI);

    // Limit normalised PI gains to int14
    int16_t int14_max = ((1 << 14) - 1);
    int16_t int14_min = -(1 << 14);

    if (pi_controller->kP > int14_max) {
        pi_controller->kP = int14_max;
    } else if (pi_controller->kP < int14_min) {
        pi_controller->kP = int14_min;
    } else if (pi_controller->kI > int14_max) {
        pi_controller->kI = int14_max;
    } else if (pi_controller->kI < int14_min) {
        pi_controller->kI = int14_min;
    }

    // Set 16-bit signed output to normalised limits
    pi_controller->out_max = (0xFFFFUL >> 1) * PARAM_MULT;
    pi_controller->out_min = -((0xFFFFUL + 1) >> 1) * PARAM_MULT;

    // Set integral sum to zero
    pi_controller->sum = 0;

    return pi_controller;
}

int16_t step_controller(int16_t sp, int16_t fb, Controller pi_controller) {
    // int16 + int16 = int17
    int32_t err = sp - fb;
    int32_t P = 0, I = 0;

    if (pi_controller->kP) {
        // int14 (kP) * int17 (err) = int31 (P)
        P = pi_controller->kP * err;
    }

    if (pi_controller->kI) {
        // int14 (kI) * int17 (err) = int31 (sum)
        pi_controller->sum += pi_controller->kI * err;

        // Limit sum to 32-bit signed value so that it saturates, never overflows.
        if (pi_controller->sum > INTEG_MAX)
            pi_controller->sum = INTEG_MAX;
        else if (pi_controller->sum < INTEG_MIN)
            pi_controller->sum = INTEG_MIN;

        // int31 (I) = int31 (sum)
        I = pi_controller->sum;
    }

    // int31 (P) + int31 (I) = int32 (out)
    int32_t out = P + I;

    // Check controller output saturation
    if (out > pi_controller->out_max) {
        out = pi_controller->out_max;
    } else if (out < pi_controller->out_min) {
        out = pi_controller->out_min;
    }

    // Denormalise compensation value
    int16_t rval = out >> PARAM_SHIFT;

    // Round denormalised value.
    if (out & (0x1 << (PARAM_SHIFT - 1))) {
        rval++;
    }

    return rval;
}

static uint32_t normalise(float in) {
    if (in > PARAM_MAX || in < 0) {
        return 0;
    }

    uint32_t param = in * PARAM_MULT;

    if (in != 0 && param == 0) {
        return 0;
    }

    return param;
}
