/**
 * FILENAME :           diagnostics.c
 * DESCRIPTION :        File to load and execute light rail diagnostic code.
 * 
 * NOTES :
 * 
 * AUTHOR :             Barry Feng   
 * 
 * START DATE :         31 Jul 20
 * 
 * CHANGES :
 * -- 31/07/2020 --     File created.
 */

#include <diagnostics.h>

extern LightRail light_rail;
extern uint8_t MMCRDData[MMC_DATA_SIZE];

void load_diag_code(uint16_t diagnostic_code[MAX_DIAGNOSTIC_CODE_SIZE]) {
    uint32_t clr_buf_count, block_num = 0;

    if (mmc_init() != 0) {
        IOSET0 = SPI_SEL;
        return;
    }

    for (block_num = 0; block_num < MAX_BLOCK_NUM; block_num++) {
        mmc_read_block(block_num);

        // Copy MMCRDATA to diag_code and clear read buffer after MMC read.
        for (clr_buf_count = 0; clr_buf_count < MMC_DATA_SIZE; clr_buf_count++)
            diagnostic_code[MMC_DATA_SIZE * block_num + clr_buf_count] =
                MMCRDData[clr_buf_count];
            MMCRDData[clr_buf_count] = 0x00;
    }
}

/**
 * This function takes in the address of the code loaded into SDRAM from the 
 * SD card and executes the code at that address.
 */
void run_diag_code(uint16_t* code) {
    CodeLocation *addr = (CodeLocation *)code;
    (*addr)();
}

/**
 * This function checks the current light rail conditions before any 
 * diagnostic code can be loaded or executed.
 * 
 * DIAGNOSTIC EXECUTION CONDITIONS --
 * - Velocity == 0
 * - Target Velocity (Setpoint) == 0 
 * - Brake State == ACTIVE
 */
int check_diag_conditions(void) {
    if (light_rail.velocity != 0) {
        return 0;
    } else if (light_rail.vel_setpoint != 0) {
        return 0;
    } else if (light_rail.brake_state == BRAKE_INACTIVE) {
        return 0;
    }

    return 1;
}

/**
 * SPI receives a block of data based on the length.
 */
void spi_read_block(uint8_t *buf, uint32_t blk_len) {
		uint32_t i = 0;
	
    while (i < blk_len) {
        *buf = spi_read();
        buf++;
				i++;
    }
}

void spi_write_block(uint8_t *buf, uint32_t blk_len) {
		uint32_t i = 0;
	
    while (i < blk_len) {
        spi_write(*buf);
        buf++;
				i++;
    }
}
