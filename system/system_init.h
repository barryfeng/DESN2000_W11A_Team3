#ifndef SYSINIT

#include <lpc24XX.h>

#define PLOCK 0x00000400
#define PWM_DC_DEF 50
#define CCLK 60000000

void init_pll(void);
void init_adc(void);
void init_dac(void);
void init_pwm(void);
void init_uart(void);

#endif