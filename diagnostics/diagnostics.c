#include <diagnostics.h>


void run_diag(void) {
    
}

void mem2mem(uint16_t *sourceAdd, uint16_t *destinationAdd, uint16_t length) {
    sourceAdd = (uint16_t *)((uint32_t)sourceAdd & (0xFFFFFFFE));
    destinationAdd = (uint16_t *)((uint32_t)destinationAdd & 0xFFFFFFFE);
    for (uint16_t i = 0; i < length; i++) {
        destinationAdd[i] = sourceAdd[i];
    }
    __DSB();
    __ISB();
}

#define SDRAM_BASE_ADDR_CODE 0xa0000001

typedef void (*functionPointer)(void);

void runCodeInSDRAM(void) {
    ((functionPointer)SDRAM_BASE_ADDR_CODE)();
}

void LED_Toggle(void) {
    while (1) {
        delay_1();
        GPIO->NOT[3] |= 1 << 3;
    }
}

void MPU_Init(void) {
    //disable MPU
    MPU->CTRL = 0x00;
    //Region0: default region 0
    MPU->RBAR = 0x00000010;
    MPU->RASR = 0x1004003f;

    //Region 1:
    //set up flash, ROM, SRAM ... as region 1
    //# 0x00000000 - 0x3FFFFFFF
    MPU->RBAR = 0x00000011;
    MPU->RASR = 0x0306E23b;

    //Region 2:
    //set up peripheral device
    //# 0x40000000 - 0x400FFFFF
    MPU->RBAR = 0x40000012;
    MPU->RASR = 0x13050033;

    //Region 3:
    //set up chip select and device
    //# 0x80000000 - 0x9000FFFF
    MPU->RBAR = 0x80000013;
    MPU->RASR = 0x1305001f;

    //Region 4:
    //set up SDRAM
    //# # 0xA0000000 - 0xA0FFFFFF
    MPU->RBAR = 0xa0000014;
    MPU->RASR = 0x0307002f;

    //Region 5:
    //set up Cortex-M4 Private Peripheral Bus
    //# 0xE0000000 - 0xE00FFFFF
    MPU->RBAR = 0xe0000015;
    MPU->RASR = 0x13040027;

    //Region 6:
    //set up Not used
    //# 0xE0000000 - 0xE00FFFFF
    MPU->RBAR = 0xe0000016;
    MPU->RASR = 0x00000000;

    //Region 7:
    //set up stack
    //#
    MPU->RBAR = 0x1000E017;
    MPU->RASR = 0x10060009;

    //enable MPU
    MPU->CTRL = 0x01;
}

int main(void) {
    uint32_t index;
    uint32_t *sdram = (uint32_t *)SDRAM_BASE_ADDR; /* SDRAM start address. */


    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    LEDInit();
    //LED_Toggle();
    BOARD_InitSDRAM();
    MPU_Init();
    mem2mem((uint16_t *)LED_Toggle, (uint16_t *)SDRAM_BASE_ADDR_CODE, 400);

    // LED_Toggle();
    runCodeInSDRAM();
}