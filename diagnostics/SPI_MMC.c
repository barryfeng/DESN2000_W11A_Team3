/*-----------------------------------------------------------------------
 * Name: SPI_MMC.C
 * Purpose: SPI and SD/MMC command interface Module
 * Version: V1.03
 * Copyright (c) 2006 NXP Semiconductor. All rights reserved.
 * 
 * This code has been modified by Barry Feng for use with the LPC2478.
 *---------------------------------------------------------------------*/

#include <SPI_MMC.h>

uint8_t MMCWRData[MMC_DATA_SIZE];
uint8_t MMCRDData[MMC_DATA_SIZE];
uint8_t MMCCmd[MMC_CMD_SIZE];
uint8_t MMCStatus = 0;

/************************** MMC Init *********************************/
/*
 * Initialises the MMC into SPI mode and sets block size(512), returns
 * 0 on success.
 */
int mmc_init(void) {
    uint32_t i;
    /* Generate a data pattern for write block */
    for (i = 0; i < MMC_DATA_SIZE; i++) {
        MMCWRData[i] = i;
    }
    MMCStatus = 0;
    IOSET0 = SPI_SEL; /* set SPI SSEL */
    /* initialise the MMC card into SPI mode by sending 80 clks on */
    /* Use MMCRDData as a temporary buffer for spi_write_block() */
    for (i = 0; i < 10; i++) {
        MMCRDData[i] = 0xFF;
    }
    spi_write_block(MMCRDData, 10);

    IOCLR0 = SPI_SEL; /* clear SPI SSEL */
    /* send CMD0(RESET or GO_IDLE_STATE) command, all the arguments
 are 0x00 for the reset command, precalculated checksum */
    MMCCmd[0] = 0x40;
    MMCCmd[1] = 0x00;
    MMCCmd[2] = 0x00;
    MMCCmd[3] = 0x00;
    MMCCmd[4] = 0x00;
    MMCCmd[5] = 0x95;
    spi_write_block(MMCCmd, MMC_CMD_SIZE);

    /* if = 1 then there was a timeout waiting for 0x01 from the MMC */
    if (mmc_response(0x01) == 1) {
        MMCStatus = IDLE_STATE_TIMEOUT;
        IOSET0 = SPI_SEL; /* set SPI SSEL */
        return MMCStatus;
    }
    /* Send some dummy clocks after GO_IDLE_STATE */
    IOSET0 = SPI_SEL; /* set SPI SSEL */
    spi_read();
    IOCLR0 = SPI_SEL; /* clear SPI SSEL */

    /* must keep sending command until zero response ia back. */
    i = MAX_TIMEOUT;
    do {
        /* send mmc CMD1(SEND_OP_COND) to bring out of idle state */
        /* all the arguments are 0x00 for command one */
        MMCCmd[0] = 0x41;
        MMCCmd[1] = 0x00;
        MMCCmd[2] = 0x00;
        MMCCmd[3] = 0x00;
        MMCCmd[4] = 0x00;
        /* checksum is no longer required but we always send 0xFF */
        MMCCmd[5] = 0xFF;
        spi_write_block(MMCCmd, MMC_CMD_SIZE);
        i--;
    } while ((mmc_response(0x00) != 0) && (i > 0));
    /* timeout waiting for 0x00 from the MMC */
    if (i == 0) {
        MMCStatus = OP_COND_TIMEOUT;
        IOSET0 = SPI_SEL; /* set SPI SSEL */
        return MMCStatus;
    }
    /* Send some dummy clocks after SEND_OP_COND */
    IOSET0 = SPI_SEL; /* set SPI SSEL */
    spi_read();
    IOCLR0 = SPI_SEL; /* clear SPI SSEL */

    /* send MMC CMD16(SET_BLOCKLEN) to set the block length */
    MMCCmd[0] = 0x50;
    MMCCmd[1] = 0x00; /* 4 uint8_ts from here is the block length */
    /* LSB is first */
    /* 00 00 00 10 set to 16 uint8_ts */
    /* 00 00 02 00 set to 512 uint8_ts */
    MMCCmd[2] = 0x00;
    /* high block length bits - 512 uint8_ts */
    MMCCmd[3] = 0x02;
    /* low block length bits */
    MMCCmd[4] = 0x00;
    /* checksum is no longer required but we always send 0xFF */
    MMCCmd[5] = 0xFF;
    spi_write_block(MMCCmd, MMC_CMD_SIZE);

    if ((mmc_response(0x00)) == 1) {
        MMCStatus = SET_BLOCKLEN_TIMEOUT;
        IOSET0 = SPI_SEL; /* set SPI SSEL */

        return MMCStatus;
    }
    IOSET0 = SPI_SEL; /* set SPI SSEL */
    spi_read();
    return 0;
}

/************************** MMC Read Block ****************************/
/*
 * Reads a 512 uint8_t block from the MMC
 * Send READ_SINGLE_BLOCK command first, wait for response come back
 * 0x00 followed by 0xFE. The call spi_read_block() to read the data
 * block back followed by the checksum.
 *
 */
int mmc_read_block(uint16_t block_number) {
    uint16_t Checksum;
    uint16_t varh, varl;
    IOCLR0 = SPI_SEL; /* clear SPI SSEL */
    varl = ((block_number & 0x003F) << 9);
    varh = ((block_number & 0xFFC0) >> 7);
    /* send MMC CMD17(READ_SINGLE_BLOCK) to read the data from MMC card */
    MMCCmd[0] = 0x51;
    /* high block address bits, varh HIGH and LOW */
    MMCCmd[1] = varh >> 0x08;
    MMCCmd[2] = varh & 0xFF;
    /* low block address bits, varl HIGH and LOW */
    MMCCmd[3] = varl >> 0x08;
    MMCCmd[4] = varl & 0xFF;
    /* checksum is no longer required but we always send 0xFF */
    MMCCmd[5] = 0xFF;
    spi_write_block(MMCCmd, MMC_CMD_SIZE);
    /* if mmc_response returns 1 then we failed to get a 0x00 response */
    if ((mmc_response(0x00)) == 1) {
        MMCStatus = READ_BLOCK_TIMEOUT;
        IOSET0 = SPI_SEL; /* set SPI SSEL */
        return MMCStatus;
    }
    /* wait for data token */

    if ((mmc_response(0xFE)) == 1) {
        MMCStatus = READ_BLOCK_DATA_TOKEN_MISSING;
        IOSET0 = SPI_SEL;
        return MMCStatus;
    }
    /* Get the block of data based on the length */
    spi_read_block(MMCRDData, MMC_DATA_SIZE);

    /* CRC uint8_ts that are not needed */
    Checksum = spi_read();
    Checksum = Checksum << 0x08 | spi_read();
    IOSET0 = SPI_SEL; /* set SPI SSEL */
    spi_read();
    return 0;
}

/***************** MMC get response *******************/
/*
 * Repeatedly reads the MMC until we get the
 * response we want or timeout
 */
int mmc_response(uint8_t response) {
    uint32_t count = 0xFFF;
    uint8_t result;
    while (count > 0) {
        result = spi_read();
        if (result == response) {
            break;
        }
        count--;
    }
    if (count == 0)
        return 1; /* Failure, loop was exited due to timeout */
    else
        return 0; /* Normal, loop was exited before timeout */
}