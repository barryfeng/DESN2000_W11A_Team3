#ifndef MASTCNTRL

#include "motion_controller.h"
#include "system_init.h"
#include "stdbool.h"
#include "stdlib.h"

typedef struct {
    bool brake_state;
    bool dms_state;
    float velocity;
} *LightRail;

#endif