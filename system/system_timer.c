#include <system_timer.h>

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

// ! TODO: BARRY make this __irq
void timer0_isr(void) {
    long int ir = T0IR;

    T0IR = ir;       // Write back to IR to clear Interrupt Flag
    VICVectAddr = 0x0;    // End of interrupt execution
}

/**
 * ARGS:
 * */

void timer0(unsigned int target, modifier_t unit) {
    T0CTCR = 0x00;                          // Set timer 0 to timer mode
    T0PR = get_prescaler(unit);             // Set prescaler (default seconds)
    T0TCR = 0x02;                           // Reset timer0

    T0MR0 = target - 1;                     // Zero indexed match
    T0MCR = (1 << 1) | (1 << 0);            // Interrupt and reset on match

    VICIntSelect &= ~(0x10);                // Timer 0 selected as IRQ
    VICIntEnable |= 0x10;                   // Timer 0 interrupt enabled
    VICVectPriority0 = 1;                   // Timer 0 interrupt priority set to maximum.
    VICVectAddr0 = (unsigned)timer0_isr;    // Timer 0 interrupt address

    T0TCR = 0x01;                           // Start timer

    while (T0TC != T0MR0);                  // Wait for timer to reset

    T0TCR = 0x00;                           // Reset timer counter
}

/**
 * ARGS:
 * */

void init_timer2(modifier_t unit) {
    T2CTCR = 0x00;               // Set timer 0 to timer mode
    T2PR = get_prescaler(unit);  // Set prescaler (default seconds)
    T2TCR = 0x02;                // Reset timer0
}

void delay_timer2(unsigned int target) {
    T2MR0 = target - 1;  // Zero indexed match
    T2MCR = (1 << 1);    // Reset on match

    T2TCR = 0x01;  // Start timer
    while (T2TC != T2MR0)
        ;  // Wait for timer to reset

    T2TCR = 0x00;  // Reset timer counter
}

/**
 * ARGS:
 * */

void init_timer3(modifier_t unit) {
    T3CTCR = 0x00;                  // Set timer 0 to timer mode
    T3PR = get_prescaler(unit);     // Set prescaler (default seconds)
    T3TCR = 0x02;                   // Reset timer0
}

void delay_timer3(unsigned int target) {
    T3MR0 = target - 1;         // Zero indexed match
    T3MCR = (1 << 1);           // Reset on match

    T3TCR = 0x01;               // Start timer
    while (T3TC != T3MR0);      // Wait for timer to reset

    T3TCR = 0x00;               // Reset timer counter
}
