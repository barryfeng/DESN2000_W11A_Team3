/**
 * FILENAME :           system_init.c
 * DESCRIPTION :        Light rail system intialisation 
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

#include "system_init.h"

/**
 * The phaselocked loop generates a 60MHz clock for the light rail system.
 * The configuration steps are below.
 */
void init_pll(void) {
    // Set clock to internal RC oscillator.
    CLKSRCSEL = 0x00;

    // Enable PLLE, Disable PLLC
    PLLCON = 0x01;

    // Set M = 59, N = 1 for PLL multiplier
    PLLCFG = 0x0003B;

    // Pass PLLFEED feed sequence for PLLCON/PLLCFG registers to take effect
    PLLFEED = 0xAA;  
    PLLFEED = 0x55;

    // Wait for the PLL to achieve lock by monitoring the PLOCK bit in the PLLSTAT register.
    while (!(PLLSTAT & PLOCK));  

    PLLCON = 0x03;

    // Pass PLLFEED feed sequence for PLLCON/PLLCFG registers to take effect
    PLLFEED = 0xAA;  
    PLLFEED = 0x55;
}

// Set ADC out to P0.23
void init_adc(void) {
    PCONP |= (1 << 12);  // Power on ADC Module
    AD0CR |= (1 << 21);  // Enable ADC
    PCLKSEL0 |= (0x01 << 24);   // Set ADC peripheral clock to CCLK (60MHz)

    PINSEL1 |= ~(0x03 << 14);
    PINSEL1 |= (0x01 << 14);  // Select P0.23 to AD0[0]
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

    PWM0LER = (1 << 0) | (1 << 1);  // Update PWM0 Latch for MR0, MR1
    PWM0TCR = (1 << 0) | (1 << 3);  // Enable PWM0 and Reset PWM0 TC
}

void init_spi(void) {
    PINSEL0 |= (0x3 << 29);         // SCLK
    PINSEL1 |= (0x3 << 2);          // MISO
    PINSEL1 |= (0x3 << 4);          // MOSI

    PINSEL1 &= ~(0x3 << 8);         // CP
    FIO0DIR |= (1 << 20);

    S0SPCR = 0x093C;            
    S0SPCCR = 0x24;
}

void init_ultrasonic(void) {
    init_timer2('u');

    PINSEL6 &= ~(0x3 << 0);
    PINSEL6 &= ~(0x3 << 2);
    PINSEL6 &= ~(0x3 << 4);
    PINSEL6 &= ~(0x3 << 6);

    FIO3DIR |= ULTRAS_ALL;
}

void spi_write(unsigned char data) {
    char result;

    FIO0PIN &= ~(1 << 20);          // Set CS_TP low to begin SPI transmission
    S0SPDR = data;                  //Transmit data on MOSI

    while ((S0SPSR >> 7) == 0);     // Transmit 0x00 on MOSI and wait to read from MISO
    result = S0SPSR;
		(void)result;               // Silence unused variable warning
	
    FIO0PIN |= (1 << 20);           // Set CS_TP high after SPI transmission complete
}

uint8_t spi_read(void) {
    FIO0PIN &= ~(1 << 20);          // Set CS_TP low to begin SPI transmission

    while ((S0SPSR >> 7) == 0);     // Transmit 0x00 on MOSI and wait to read from MISO

    FIO0PIN |= (1 << 20);           // Set CS_TP high after SPI transmission complete
    
    return S0SPSR;                  // Return read result
}
