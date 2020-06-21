/*
 * FILENAME :       motion_controller.c
 *
 * DESCRIPTION :    PWM Generation for DESN2000 ELEC W11A Group 3 
 *
 * NOTES :
 *
 * AUTHOR :         Barry Feng            
 *
 * START DATE :     20 Jun 20
 *
 * CHANGES :
 *   -- 20/06/2020 --     File created.
 */

#include <LPC24XX.h>

#define PLOCK 0x00000400
#define DC_DEF 5

void init_pll(void)                     // Initialise PLL for clock generation
{
    CLKSRCSEL = 0x00;                   // Set clock to internal RC oscillator.
    PLLCON = 0x01;                      // Enable PLLE, Disable PLLC
    PLLCFG = 0x0003B;                   // Set M = 59, N = 1 for PLL multiplier

    PLLFEED = 0xAA;                     // Pass PLLFEED feed sequence for PLLCON and PLLCFG registers to take effect
    PLLFEED = 0x55;

    while (!(PLLSTAT & PLOCK));         // Wait for the PLL to achieve lock by monitoring the PLOCK bit in the PLLSTAT register.

    PLLCON = 0x03;
    PLLFEED = 0xAA;                     // Pass PLLFEED feed sequence for PLLCON and PLLCFG registers to take effect
    PLLFEED = 0x55;
    PCLKSEL0 = 0x01;                    // Supply CCLK to PCLK
}

void init_pwm(void) {
    PCONP |= (1 << 5);                  // Power on PWM0 power/clock control
    PWM0PCR |= (1 << 9);                // Set PWM to PWM0[1]

    PINSEL2 |= (3 << 6);                // Set P1.3 for PWM Output.

    PWM0TCR = (1 << 1);                 // Reset PWM TC and PWM PR.
    PWM0PR = 59999;                     // Set prescale register to (60000 - 1) (ms resolution).
    PWM0MCR = (1 << 0) | (1 << 1);      // Set PWM Match Control Register to reset PWMTC on match with PWMMR0.

    PWM0MR0 |= 10;                      // Set PWM period to 10 ms.
    PWM0MR1 |= DC_DEF;                  // Set PWM pulse width to default (5ms).
    
    PWM0LER = (1 << 0);                 // Enable PWM latch.
}

void start_pwm(void) {
                                        // ? NOTE FOR BARRY: we use timer3 to prevent external memory startup conflicts
    T3TCR = 0x02;                       // Reset timer3
    T3IR |= 0x01;                       // Clear interrupt register
    T3MR0 = 10;                         // PWM period matching
    T3MCR = (1 << 0) | (1 << 1);        // Reset counter and set interrupt on match
    T3TCR = (1 << 0);                   // Start timer

    init_pll();
    init_pwm();
    PWM0TCR |= 0x00000009;

    while (1) {
        if (T1IR & 0x1) {
                                        // LOAD PID COMPENSATION HERE
            PWM0MR1 = 6;                // Set new speed
            PWM0LER = (1 << 1);         // Latch to new speed
        }
    }
}