#include <diagnostics.h>

uint16_t load_diag_code(void) {
    uint32_t read_buf_count, clr_buf_count, block_num = 0;
    uint16_t diag_code[MAX_CODE_SIZE];

    if (mmc_init() != 0) { 
        IOSET0 = SPI_SEL;
        return 0;
    }

    for (block_num = 0; block_num < MAX_BLOCK_NUM; block_num++) {
        mmc_read_block(block_num);

        // Copy MMCRDATA to diag_code and clear read buffer after MMC read.
        for (clr_buf_count = 0; clr_buf_count < MMC_DATA_SIZE; clr_buf_count++)
            diag_code[MMC_DATA_SIZE * block_num + clr_buf_count] = 
                MMCRDData[clr_buf_count];
            MMCRDData[clr_buf_count] = 0x00;
    }

    return diag_code;
}

/**
 * This function takes in the address of the code loaded into SDRAM from the 
 * SD card and executes the code at that address.
 */
void run_diag_code(unsigned int *code) {
    void (*func_ptr)(void) = &code;
    (*func_ptr)();
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
    for (uint32_t i = 0; i < blk_len; i++) {
        *buf = spi_read();
        buf++;
    }
}

void spi_write_block(uint8_t *buf, uint32_t blk_len) {
    for (int i = 0; i < blk_len; i++) {
        spi_write(*buf);
        buf++;
    }
}