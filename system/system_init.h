#ifndef SYSINIT

#define SYSINIT

#include <lpc24XX.h>
#include "stdint.h"
#include "user_interface/LCD/lcd_main.h"

#define PLOCK 0x00000400
#define PWM_DC_DEF 50
#define CCLK 60000000

void init_pll(void);
void init_adc(void);
void init_pwm(void);
void init_spi(void);
void init_ultrasonic(void);
void spi_write(unsigned char data);
uint8_t spi_read(void);


#endif
