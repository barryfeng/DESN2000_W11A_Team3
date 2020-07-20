#define INTEG_MAX (INT32_MAX)
#define INTEG_MIN (INT32_MIN)

#define PARAM_SHIFT 8
#define PARAM_BITS 16
#define PARAM_MAX (((0x1UL << PARAM_BITS) - 1) >> PARAM_SHIFT)
#define PARAM_MULT (((0x1UL << PARAM_BITS)) >> (PARAM_BITS - PARAM_SHIFT))

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

uint32_t normalise(float in) {
    if (in > PARAM_MAX || in < 0) {
        return 0;
    }

    uint32_t param = in * PARAM_MULT;

    if (in != 0 && param == 0) {
        return 0;
    }

    return param;
}

//  Test function
int main(void) {
    int32_t out_max = (0xFFFFUL >> 1) * PARAM_MULT;
    int32_t out_min = -((0xFFFFUL + 1) >> 1) * PARAM_MULT;
    printf("Normalise: %d\n", normalise(10.0));
    printf("Denormal: %d\n", normalise(10.0) >> PARAM_SHIFT);
    printf("ParamMax: %d\n", PARAM_MAX);
    printf("PARAM_MULT: %d\n", PARAM_MULT);
    printf("min: %d, max %d\n", out_min, out_max);
    printf("SHIFT: %d\n", -(1 << 14));
    printf("Rounding: %d", 0x1ULL << (PARAM_SHIFT - 1));
}
