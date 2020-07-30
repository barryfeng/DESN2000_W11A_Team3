/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2008 Embedded Artists AB
 *
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/


#include "lpc24xx.h"
#include "sdram.h"
#include "../delay.h"

/******************************************************************************
 * Implementation of public functions
 *****************************************************************************/

/*****************************************************************************
 *
 * Description:
 *    Initialize external SDRAM
 *
 ****************************************************************************/
void 
sdramInit(void)
{
  volatile unsigned int i, dummy = dummy;

//  SCS     &= ~0x00000002;
//  SCS     |= 0x00000002;
  EMC_CTRL = 0x00000001;
  PCONP   |= 0x00000800;    /* Turn on EMC PCLK */
  PINSEL4  = 0x10000000;
#ifdef USE_32_BIT_DATABUS
  PINSEL5  = 0x55010115;
  PINSEL7  = 0x55555555;
#else
  PINSEL5  = 0x05050555;
#endif
  PINSEL6  = 0x55555555;
  PINSEL8  = 0x55555555;
  PINSEL9  = 0x50555555;

  //all registers...
#ifdef USE_32_BIT_DATABUS
  EMC_DYN_RP     = 1;
  EMC_DYN_RAS    = 3;
  EMC_DYN_SREX   = 5;
  EMC_DYN_APR    = 1;
  EMC_DYN_DAL    = 5;
  EMC_DYN_WR     = 1;
  EMC_DYN_RC     = 5;
  EMC_DYN_RFC    = 5;
  EMC_DYN_XSR    = 5;
  EMC_DYN_RRD    = 1;
  EMC_DYN_MRD    = 1;
  EMC_DYN_RD_CFG = 1;
    
  //
  EMC_DYN_RASCAS0 = 0x00000202;

  //
  EMC_DYN_CFG0 = 0x00005488;
#else
  EMC_DYN_RP     = 2;
  EMC_DYN_RAS    = 3;
  EMC_DYN_SREX   = 7;
  EMC_DYN_APR    = 2;
  EMC_DYN_DAL    = 5;
  EMC_DYN_WR     = 1;
  EMC_DYN_RC     = 5;
  EMC_DYN_RFC    = 5;
  EMC_DYN_XSR    = 7;
  EMC_DYN_RRD    = 1;
  EMC_DYN_MRD    = 2;
  EMC_DYN_RD_CFG = 1;
    
  //
  EMC_DYN_RASCAS0 = 0x00000303;
    
  //
  EMC_DYN_CFG0 = 0x00000680;
#endif

  IODIR1 |= 0x00000020;  //Make P1.5 output and power cycle SD/MMC power
  IOSET1  = 0x00000020;  //Make P1.5 high = turn off power for SD/MMC card
    
  //wait 100mS
  mdelay(100);

  //Send command: NOP
  EMC_DYN_CTRL = 0x00000183;
    
  //wait 200mS
  mdelay(200);
    
  IOCLR1  = 0x00000020;  //Make P1.5 low = turn on power for SD/MMC card

  //Send command: PRECHARGE-ALL, shortest possible refresh period
  EMC_DYN_CTRL = 0x00000103;
  EMC_DYN_RFSH = 0x00000002;
    
  //wait 128 ABH clock cycles
  for(i=0; i<0x40; i++)
    __asm("nop");
    
  //Set correct refresh period
  EMC_DYN_RFSH = (unsigned int)(Fcclk / 250 / 8192) + 1;
    
  //Send command: MODE
  EMC_DYN_CTRL = 0x00000083;
    
  //Set mode register in SDRAM
#ifdef USE_32_BIT_DATABUS
  dummy = *((volatile unsigned int*)(SDRAM_BASE | (0x22 << 11)));
#else
  dummy = *((volatile unsigned int*)(SDRAM_BASE | (0x33 << 12)));
#endif

  //Send command: NORMAL
  EMC_DYN_CTRL = 0x00000000;

  //Enable buffer
  EMC_DYN_CFG0 |= 0x00080000;

  //initial system delay
  mdelay(1);

  EMC_STA_WAITWEN0  = 0x2;
  EMC_STA_WAITOEN0  = 0x2;
  EMC_STA_WAITRD0   = 0x1f;
  EMC_STA_WAITPAGE0 = 0x1f;
  EMC_STA_WAITWR0   = 0x1f;
  EMC_STA_WAITTURN0 = 0xf;
  EMC_STA_CFG0      = 0x00000081;

  EMC_STA_WAITWEN1  = 0x2;
  EMC_STA_WAITOEN1  = 0x2;
  EMC_STA_WAITRD1   = 0x8;
  EMC_STA_WAITPAGE1 = 0x1f;
  EMC_STA_WAITWR1   = 0x8;
  EMC_STA_WAITTURN1 = 0xf;
  EMC_STA_CFG1      = 0x00000080;  
}



