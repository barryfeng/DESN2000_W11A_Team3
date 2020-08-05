#ifndef __SPEED_LIMIT_H__
#define __SPEED_LIMIT_H__

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

#define SPEED_LIM_DATA_SIZE 30
#define Q_SHIFT 16

void init_vel_limit_data(void);
uint32_t get_vel_limit_data(uint8_t loc_id);

#endif
