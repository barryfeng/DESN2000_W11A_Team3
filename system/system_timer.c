#include "lpc24xx.h"

// Initialize DAC.
// This should be the same as the previous exercise.
void setup_DAC(void) {
    PINSEL1 &= ~(0X03 << 20);
    PINSEL1 |= (0X02 << 20);
}

// udelay should delay the processor for 'delay_in_us' number of microseconds.
void mdelay0(unsigned int delay_ms) {
    T0TCR = 0x02;           // Reset timer0
    T0PR = 0xEA5F;          // Load 59999 into PR (60MHz/60000 = 1 millisecond delay)
    T0MR0 = delay_ms;
    T0MCR = (1 << 1);       // Reset on match
    T0TCR = 0x01;           // Start timer

    while (T0TC != T0MR0);  // Wait for timer to reset

    T0TC = 0x00;            // Reset timer counter
}
