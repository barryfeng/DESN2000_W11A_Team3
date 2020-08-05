#ifndef SYSTIME

#define SYSTIME

#include "system_init.h"
#include "motion_control/motion_controller.h"
#include "user_interface/touchscreen/touch_screen_read.h"
#include "logging/access_data.h"

/**
 * The following define the timer modifiers which can be used to set the
 * timer prescalers.
 */
typedef char modifier_t;
#define MODIFIER_SEC 1
#define MODIFIER_MILLI 1000
#define MODIFIER_MICRO 1000000

int get_prescaler(modifier_t target_modifier);

__irq void master_isr_handler(void);
void start_master_isr(unsigned int target, modifier_t unit);
void stop_master_isr(void);

void init_timer2(modifier_t unit);
void delay_timer2(unsigned int target);
void start_timer2_stopwatch(void);
uint32_t split_timer2_stopwatch(void);
uint32_t reset_timer2_stopwatch(void);

void init_timer3(modifier_t unit);
void delay_timer3(unsigned int target);

#endif
