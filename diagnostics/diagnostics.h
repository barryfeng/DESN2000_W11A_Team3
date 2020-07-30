#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H


#include <LPC24XX.h>
#include <master_controller.h>
#include <SPI_MMC.h>

#define MAX_BLOCK_NUM 0x80
#define MAX_CODE_SIZE 0xFFFF

extern LightRail light_rail;
extern uint8_t MMCRDData[MMC_DATA_SIZE];

uint8_t load_diag_code(void);
void run_diag_code(unsigned int *code);
int check_diag_conditions(void);
void spi_read_block(uint8_t *buf, uint32_t blk_len);
void spi_write_block(uint8_t *buf, uint32_t blk_len);

#endif