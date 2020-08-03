/*----------------------------------------------------------------------
 * Name: SPI_MMC.H
 * Purpose: SPI mode SD/MMC card interface driver
 * Version: V1.03
 * Copyright (c) 2006 NXP Semiconductor. All rights reserved.
 *---------------------------------------------------------------------*/
#ifndef MMC_H__
#define MMC_H__

#include "../LPC24XX.h"
#include "stdint.h"
#include "system/system_init.h"
#include "diagnostics.h"

/* SPI select pin */
#define SPI_SEL 0x00100000
/* The SPI data is 8 bit long, the MMC use 48 bits, 6 bytes */
#define MMC_CMD_SIZE 6
/* The max MMC flash size is 256MB */
#define MMC_DATA_SIZE 512 /* 16-bit in size, 512 bytes */

#define MAX_TIMEOUT 0xFF
#define IDLE_STATE_TIMEOUT 10

#define OP_COND_TIMEOUT 2
#define SET_BLOCKLEN_TIMEOUT 3
#define WRITE_BLOCK_TIMEOUT 4
#define WRITE_BLOCK_FAIL 5
#define READ_BLOCK_TIMEOUT 6
#define READ_BLOCK_DATA_TOKEN_MISSING 7
#define DATA_TOKEN_TIMEOUT 8
#define SELECT_CARD_TIMEOUT 9
#define SET_RELATIVE_ADDR_TIMEOUT 10

int mmc_init(void);
int mmc_response(uint8_t response);
int mmc_read_block(uint16_t block_number);
int mmc_write_block(uint16_t block_number);
int mmc_wait_for_write_finish(void);

#endif /* MMC_H__ */
