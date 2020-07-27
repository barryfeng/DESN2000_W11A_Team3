#define INTEG_MAX (INT32_MAX)
#define INTEG_MIN (INT32_MIN)

#define PARAM_SHIFT 8
#define PARAM_BITS 16
#define PARAM_MAX (((0x1UL << PARAM_BITS) - 1) >> PARAM_SHIFT)
#define PARAM_MULT (((0x1UL << PARAM_BITS)) >> (PARAM_BITS - PARAM_SHIFT))

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct controller {
    int32_t kP;
    int32_t kI;
    int32_t out_max;
    int32_t out_min;
    int32_t sum;
    int8_t initialised;
} Controller;

Controller init_controller(float kP, float kI) {
    Controller pi_controller;
    int16_t int14_max = ((1 << 14) - 1);
    int16_t int14_min = -(1 << 14);

    memset(&pi_controller, '0', sizeof(Controller));

    // Set PI parameters
    pi_controller.kP = (kP);
    pi_controller.kI = (kI);

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
    pi_controller.out_min = (int32_t)(-((0xFFFFUL + 1) >> 1) * PARAM_MULT);

    // Set integral sum to zero
    pi_controller.sum = 0;

    pi_controller.initialised = 1;

    return pi_controller;
}

void recur(void) {
    static Controller pi_controller;

    if (!pi_controller.initialised) {
        pi_controller = init_controller(1.0, 1.2);
        printf("Initialising");
    }

}

//  Test function
int main(void) {
    printf("Starting\n");
    for (int i = 0; i < 10; i++) {
        printf("\nCycle: %d - ", i);
        recur();
    }

    return 0;
}



