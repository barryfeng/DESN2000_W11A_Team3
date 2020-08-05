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

    /** 
     * Wait for the PLL to achieve lock by monitoring the PLOCK bit in 
     * the PLLSTAT register.
     */
    while (!(PLLSTAT & PLOCK));  

    PLLCON = 0x03;

    // Pass PLLFEED feed sequence for PLLCON/PLLCFG registers to take effect
    PLLFEED = 0xAA;  
    PLLFEED = 0x55;
}

// Set ADC out to P0.23
void init_adc(void) {
    // Power on ADC Module
    PCONP |= (1 << 12);

    // Enable ADC
    AD0CR |= (1 << 21);

    // Set ADC peripheral clock to CCLK (60MHz)
    PCLKSEL0 |= (0x01 << 24);

    // Select P0.23 to AD0[0]
    PINSEL1 |= ~(0x03 << 14);
    PINSEL1 |= (0x01 << 14);
}

// Set PWM out to GPIO P1.3
void init_pwm(void) {
    // Power on PWM0 power/clock control
    PCONP |= (1 << 5);

    // Set PWM to PWM0[1]
    PWM0PCR |= (1 << 9);

    // Set ADC peripheral clock to CCLK (60MHz)
    PCLKSEL0 |= (0x01 << 10);

    // Clear P1.3
    PINSEL2 &= ~(0x03 << 6);

    // Set P1.3 for PWM Output
    PINSEL2 |= (0x03 << 6); 

    // Set prescale register to (60 - 1) (microsec resolution)
    PWM0PR = 59;

    // Set PWM Match Control Register to reset PWMTC on match with PWMMR0
    PWM0MCR = (1 << 1);
    PWM0LER |= (1 << 0) | (1 << 1);

    // Set PWM period to 32.768 ms (2^15)
    PWM0MR0 = 32768;

    // Set PWM pulse width to default duty cycle (0)
    PWM0MR1 = 0;

    // Reset PWM TC
    PWM0TCR = (1 << 1);

    // Update PWM0 Latch for MR0, MR1
    PWM0LER = (1 << 0) | (1 << 1);

    // Enable PWM0 and Reset PWM0 TC
    PWM0TCR = (1 << 0) | (1 << 3);
}

void init_spi(void) {
    // SCLK
    PINSEL0 |= (0x3 << 29);

    // MISO
    PINSEL1 |= (0x3 << 2);

    // MOSI
    PINSEL1 |= (0x3 << 4);

    // CP
    PINSEL1 &= ~(0x3 << 8);    
    FIO0DIR |= (1 << 20);

    S0SPCR = 0x093C;            
    S0SPCCR = 0x24;
}

void init_ultrasonic(void) {
    /**
     * Timer2 is initialised in microseconds to assist with HC-SR4 sampling 
     * routine.
     */
    init_timer2('u');

    /**
     * The following pin selects set the appropriate pins for the HC-SR04
     * sensors to GPIO mode. The FIO3DIR sets these pins to outputs.
     */
    PINSEL6 &= ~(0x3 << 0);
    PINSEL6 &= ~(0x3 << 2);
    PINSEL6 &= ~(0x3 << 4);
    PINSEL6 &= ~(0x3 << 6);

    FIO3DIR |= ULTRAS_ALL;
}

void spi_write(unsigned char data) {
    char result;

    // Set CS_TP low to begin SPI transmission
    FIO0PIN &= ~(1 << 20);

    //Transmit data on MOSI
    S0SPDR = data;

    // Transmit 0x00 on MOSI and wait to read from MISO
    while ((S0SPSR >> 7) == 0);     
    result = S0SPSR;
	(void)result;

    // Set CS_TP high after SPI transmission complete
    FIO0PIN |= (1 << 20);          
}

uint8_t spi_read(void) {
    // Set CS_TP low to begin SPI transmission
    FIO0PIN &= ~(1 << 20);

    // Transmit 0x00 on MOSI and wait to read from MISO
    while ((S0SPSR >> 7) == 0);

    // Set CS_TP high after SPI transmission complete
    FIO0PIN |= (1 << 20);

    // Return read result
    return S0SPSR;                  
}
