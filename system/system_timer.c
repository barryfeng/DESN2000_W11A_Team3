/**
 * FILENAME :           system_timer.c
 * DESCRIPTION :        Light rail timer intialisation 
 * 
 * NOTES :
 * 
 * AUTHOR :             Barry Feng   
 * 
 * START DATE :         20 June 20
 * 
 * CHANGES :
 * -- 20/06/2020 --     File created.
 */

#include <system_timer.h>

extern LightRail light_rail;

/** 
 * This function returns the appropriate prescaler value to target the
 * time magnitude given as the parameter target_modifier.
 */
int get_prescaler(modifier_t target_modifier) {
    int prescale = 0;
    
    if (target_modifier == 'm') {
        prescale = CCLK / MODIFIER_MILLI;
    } else if (target_modifier == 'u') {
        prescale = CCLK / MODIFIER_MICRO;
    } else if (target_modifier == 's') {
        prescale = CCLK / MODIFIER_SEC;
    } else {
        prescale = CCLK / MODIFIER_SEC;
    }

    return prescale - 1;
}

/**
 * This function is the IRQ handler for the system. All functions within
 * this handler are run every at the CLOCK_CYCLE defined in 
 * master_controller.h.
 */
__irq void master_isr_handler(void) {
    long int ir = T0IR;

    run_controller();
    touch_screen_press();
    lcd_run();
    store_data();

    // Write back to IR to clear Interrupt Flag
    T0IR = ir;

    // End of interrupt execution
    VICVectAddr = 0x0;
}

/**
 * This function starts the system ISR. It defines the interrupt handler and
 * uses timer0 for the interrupt cycle. When run, the master_tmr_state value
 * in the light rail central structure is also set to 1.
 */
void start_master_isr(unsigned int target, modifier_t unit) {
    // Set timer 0 to timer mode
    T0CTCR = 0x00;

    // Set prescaler (default seconds)
    T0PR = get_prescaler(unit);

    // Reset timer0
    T0TCR = 0x02;

    // Zero indexed match
    T0MR0 = target - 1;

    // Interrupt and reset on match
    T0MCR = (1 << 1) | (1 << 0);

    // Timer 0 selected as IRQ
    VICIntSelect &= ~(0x1 << 4);

    // Timer 0 interrupt enabled
    VICIntEnable |= (0x1 << 4);

    // Timer 0 interrupt priority set to maximum.
    VICVectPriority0 = 1;

    // Timer 0 interrupt address
    VICVectAddr0 = (unsigned)master_isr_handler;

    // Start timer
    T0TCR = 0x01;

    // Wait for timer to match
    while (T0TC != T0MR0);

    // Reset timer counter
    T0TCR = 0x02;

    light_rail.master_tmr_state = 1;
}

/** 
 * This function stops the system ISR by reseting, disabling the timer0
 * counter and setting master_tmr_state to 0.
 */
void stop_master_isr(void) {
    T0TCR = 0x2;
    light_rail.master_tmr_state = 0;
}


/**
 * This function initialises timer2 to a time magnitude given by the 
 * parameter unit. Accepted values include ('s' for seconds, 'm' for
 * milliseconds and 'u' for microseconds).
 */
void init_timer2(modifier_t unit) {
    // Set timer 2 to timer mode
    T2CTCR = 0x0;

    // Set timer prescaler (default seconds)
    T2PR = get_prescaler(unit);

    // Reset timer2
    T2TCR = 0x2;
}

/**
 * This function causes a system delay of time (parameter: target) using 
 * timer2.
 */
void delay_timer2(unsigned int target) {
    // Set match register to target - 1 as T2MR0 is zero-indexed.
    T2MR0 = target - 1;

    // Reset timer2 on match
    T2MCR = (1 << 1);

    // Start timer2
    T2TCR = 0x1;

    // Wait for timer2 to match
    while (T2TC != T2MR0);

    // Reset and disable timer2 counter on completion
    T2TCR = 0x2;
}

/**
 * This function starts timer2 in a "stop-watch mode". This is used to
 * measure a specified time after the start_timer2_stopwatch has been
 * called.
 */
void start_timer2_stopwatch(void) {
    T2TCR = 0x1;
}

/**
 * This function splits the timer2_stopwatch time by returning the current
 * elapsed stopwatch time but does not stop the stopwatch.
 */
uint32_t split_timer2_stopwatch(void) {
    return (uint32_t) T2TC;
}

/**
 * This function resets the timer2_stopwatch time and returns the current
 * elapsed stopwatch time.
 */
uint32_t reset_timer2_stopwatch(void) {
    uint32_t count = T2TC;
    T2TCR = 0x2;  // Reset and disable timer counter

    return count;
}

/**
 * This function initialises timer3 to a time magnitude given by the 
 * parameter unit. Accepted values include ('s' for seconds, 'm' for
 * milliseconds and 'u' for microseconds).
 */
void init_timer3(modifier_t unit) {
    // Set timer3 to timer mode
    T3CTCR = 0x00;

    // Set timer3 prescaler (default seconds)
    T3PR = get_prescaler(unit);

    // Reset timer3
    T3TCR = 0x02;
}

/**
 * This function causes a system delay of time (parameter: target) using 
 * timer3.
 */
void delay_timer3(unsigned int target) {
    // Set match register to target - 1 as T2MR0 is zero-indexed.
    T3MR0 = target - 1;

    // Reset timer3 on match
    T3MCR = (1 << 1);

    // Start timer3
    T3TCR = 0x01;

    // Wait for timer3 to match
    while (T3TC != T3MR0);

    // Reset timer3 counter
    T3TCR = 0x02;
}
