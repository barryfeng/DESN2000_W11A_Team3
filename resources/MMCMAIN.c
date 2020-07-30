/*----------------------------------------------------------------------
 * Name: MMCMAIN.C
 * Purpose: SD/MMC Card Access Demo
 * Version: V1.03
 * Copyright (c) 2006 NXP Semiconductor. All rights reserved.
 *---------------------------------------------------------------------*/
#include "mmcmain.h"

#include "SPI_MMC.h"

extern uint8_t MMCWRData[MMC_DATA_SIZE];
extern uint8_t MMCRDData[MMC_DATA_SIZE];
/* Main Program */
int main(void) {
    uint32_t i, BlockNum = 0;


    /* write, read back, and compare the complete 64KB on the MMC
 * card each block is 512 uint8_ts, the total is 512 * 128 */
    for (BlockNum = 0; BlockNum < MAX_BLOCK_NUM; BlockNum++) {
        IOCLR1 = LED_MSK;
        IOSET1 = LED_WR;
        if (mmc_write_block(BlockNum) == 0) {
            IOCLR1 = LED_MSK;
            IOSET1 = LED_RD;
            mmc_read_block(BlockNum);
        } else {
            IOSET0 = SPI_SEL; /* set SSEL to high */
            while (1)
                ; /* Very bad happened */
        }

        for (i = 0; i < MMC_DATA_SIZE; i++) /* clear read buffer */
            MMCRDData[i] = 0x00;
    }
    IOCLR1 = LED_MSK;
    IOSET1 = LED_DONE;
    while (1)
        ; /* Loop forever */
}