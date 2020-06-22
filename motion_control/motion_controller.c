/**
 * FILENAME :        motion_controller.c
 * DESCRIPTION :     PWM Generation for DESN2000 ELEC W11A Group 3 
 * 
 * NOTES :
 * 
 * AUTHOR :    Barry Feng   
 * 
 * START DATE :    20 Jun 20
 * 
 * CHANGES :
 * -- 20/06/2020 --     File created.
 */

#include <LPC24XX.h>
#include <stdint.h>

double get_voltage(void) {
    static uint32_t result;
    static double voltage;

    while (1) {
        AD0CR = AD0CR | (1 << 24);  // start adc start
        while (!(AD0DR1 & 0x80000000));  // wait for adc to finish
        result = AD0DR1;
        result = (result >> 6);
        result = (result & 0x000003FF);
        voltage = ((result / 1023.0) * 3.3);  // Convert ADC value to equivalent voltage
    }
}

void start_pwm(void) {
                                        // ? NOTE FOR BARRY: we use timer3 to prevent external memory startup conflicts
    T3TCR = 0x02;                       // Reset timer3
    T3IR |= 0x01;                       // Clear interrupt register
    T3MR0 = 10;                         // PWM period matching
    T3MCR = (1 << 0) | (1 << 1);        // Reset counter and set interrupt on match
    T3TCR = (1 << 0);                   // Start timer

    PWM0TCR |= 0x00000009;

    while (1) {
        if (T1IR & 0x1) {
            get_voltage();
                                        // ? LOAD PID COMPENSATION HERE
            PWM0MR1 = 6;                // Set new speed
            PWM0LER = (1 << 1);         // Latch to new speed
        }
    }
}