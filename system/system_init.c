#include "system_init.h"

void init_pll(void) {  // Initialise PLL for clock generation
    CLKSRCSEL = 0x00;  // Set clock to internal RC oscillator.
    PLLCON = 0x01;     // Enable PLLE, Disable PLLC
    PLLCFG = 0x0003B;  // Set M = 59, N = 1 for PLL multiplier

    PLLFEED = 0xAA;  // Pass PLLFEED feed sequence for PLLCON and PLLCFG registers to take effect
    PLLFEED = 0x55;

    // Wait for the PLL to achieve lock by monitoring the PLOCK bit in the PLLSTAT register.
    while (!(PLLSTAT & PLOCK));  

    PLLCON = 0x03;
    PLLFEED = 0xAA;  // Pass PLLFEED feed sequence for PLLCON and PLLCFG registers to take effect
    PLLFEED = 0x55;
    // PCLKSEL0 = 0x01;  // Supply CCLK to PCLK Watchdog
}

// Set ADC out to P0.23
void init_adc(void) {
    PCONP |= (1 << 12);  // Power on ADC Module
    AD0CR |= (1 << 21);  // Enable ADC
    PCLKSEL0 |= (0x01 << 24);   // Set ADC peripheral clock to CCLK (60MHz)

    PINSEL1 |= ~(0x03 << 14);
    PINSEL1 |= (0x01 << 14);  // Select P0.23 to AD0[0]
}

void init_dac(void) {
    PINSEL1 |= ~(0x03 << 20);
    PINSEL1 |= (0x02 << 20);
}

// Set PWM out to GPIO P1.3
void init_pwm(void) {
    PCONP |= (1 << 5);    // Power on PWM0 power/clock control
    PWM0PCR |= (1 << 9);  // Set PWM to PWM0[1]

    PCLKSEL0 |= (0x01 << 10);  // Set ADC peripheral clock to CCLK (60MHz)

    PINSEL2 &= ~(0x03 << 6);  // Clear P1.3
    PINSEL2 |= (0x03 << 6);  // Set P1.3 for PWM Output.

    PWM0PR = 59;                    // Set prescale register to (60 - 1) (microsec resolution).
    PWM0MCR = (1 << 1);  // Set PWM Match Control Register to reset PWMTC on match with PWMMR0.
    PWM0LER |= (1 << 0) | (1 << 1); 

    PWM0MR0 = 32768;                // Set PWM period to 32.768 ms (2^15).
    PWM0MR1 = 0;                    // Set PWM pulse width to default duty cycle (0).
    PWM0TCR = (1 << 1);             // Reset PWM TC.
}

// Set brakes to GPIO P1.0
void init_brakes(void) {
    PINSEL2 &= ~((1 << 0) | (1 << 1));
    PINSEL2 |= 0x0;

    FIO1DIR |= (1 << 0);
}

// When the use_UART0.c file is called, UART0 will be configured.
/*
void init_uart(void) {
    // Use UART0

    PCONP |= (1 << 3);          // Enable power for UART
    PCLKSEL0 |= (0b11 << 6);    // Set the clock signal that will be suplied to UART0
    U0LCR |= (1 << 7);          // Enable access to Divisor Latches (DLAB = 1)
    U0FCR |= 1;                 // Enable FIFO for UART0 Rx and Tx

    //U0LCR |= (1 << 7);        // This enables UART interrupts (DLAB = 0)
}
*/