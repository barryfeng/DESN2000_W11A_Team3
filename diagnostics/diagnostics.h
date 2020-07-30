#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H


#include <LPC24XX.h>
#include <master_controller.h>

extern LightRail light_rail;

void run_diag_code(unsigned int *code);
int check_diag_conditions(void);

#endif