#ifndef FIXED_POINT_PID_H

#define FIXED_POINT_PID_H

#define INTEG_MAX (INT32_MAX)
#define INTEG_MIN (INT32_MIN)

#define PARAM_SHIFT 8
#define PARAM_BITS 16
#define PARAM_MAX (((0x1UL << PARAM_BITS) - 1) >> PARAM_SHIFT)
#define PARAM_MULT (((0x1UL << PARAM_BITS)) >> (PARAM_BITS - PARAM_SHIFT))

#include <stdint.h>
#include <string.h>

typedef struct controller {
    int32_t kP;
    int32_t kI;
    int32_t out_max;
    int32_t out_min;
    int32_t sum;   
} Controller;

Controller init_controller(float, float);
int16_t step_controller(int16_t, int16_t, Controller);
static uint32_t f_to_q(float);

#endif
