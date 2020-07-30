/*----------------------------------------------------------------------
 * Name: MMCMAIN.H
 * Purpose: SD/MMC Acess demo Definitions
 * Version: V1.03
 * Copyright (c) 2006 NXP Semiconductor. All rights reserved.
 *---------------------------------------------------------------------*/
#define CCLK 60000000 /* CPU Clock */
/* BLOCK number of the MMC card */
/* LED Definitions */
#define LED_MSK 0x00FF0000 /* P1.16..23 */
#define LED_RD 0x00010000 /* P1.16 */
#define LED_WR 0x00020000 /* P1.17 */
#define LED_CFG 0x00400000 /* P1.22 */
#define LED_DONE 0x00800000 /* P1.23 */
