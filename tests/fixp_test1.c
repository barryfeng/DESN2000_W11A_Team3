#define INTEG_MAX (INT32_MAX)
#define INTEG_MIN (INT32_MIN)

#define PARAM_SHIFT 8
#define PARAM_BITS 16
#define PARAM_MAX (((0x1UL << PARAM_BITS) - 1) >> PARAM_SHIFT)
#define PARAM_MULT (((0x1UL << PARAM_BITS)) >> (PARAM_BITS - PARAM_SHIFT))

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


//  Test function
int main(void) {
    float t = 775;
    int32_t t_int = t;
    printf("float2: %f\n", t * 3.3 / 1023);

    printf("divt: %f\n", (float) (t_int << 16) / 310);
    printf("divf: %d\n", ((0x34DA * (int16_t) t) >> (22 - 16)));

    int32_t vel = (int32_t)(((0x34DA * (int16_t)t) >> (22 - 16)) * 11);
    printf("Vel: %d\n", vel);
    printf("vELMS: %d\n", vel >> 16);
}


