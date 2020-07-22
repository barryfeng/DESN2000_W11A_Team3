#ifndef SYSTIME

#define SYSTIME

#include "system_init.h"

typedef char modifier_t;
#define MODIFIER_SEC 1
#define MODIFIER_MILLI 1000
#define MODIFIER_MICRO 1000000

int get_prescaler(modifier_t target_modifier);

void timer0_isr(void);
void timer0(unsigned int target, modifier_t unit);

void init_timer2(modifier_t unit);
void delay_timer2(unsigned int target);

void init_timer3(modifier_t unit);
void delay_timer3(unsigned int target);

#endif
