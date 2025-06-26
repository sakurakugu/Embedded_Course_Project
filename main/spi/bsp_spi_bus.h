#pragma once

#include "stm32f10x.h"
#include "bsp_lcd.h"
#include <stdio.h>

//#define SOFT_SPI		/* 定义此行表示使用GPIO模拟SPI接口 */
#define HARD_SPI		/* 定义此行表示使用CPU的硬件SPI接口 */


/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

#define Dummy_Byte                0x00


void bsp_InitSPIBus(void);

uint8_t bsp_spiWrite0(uint8_t _ucByte);
uint8_t bsp_spiRead0(void);

uint8_t bsp_spiWrite1(uint8_t _ucByte);
uint8_t bsp_spiRead1(void);



void bsp_SPI_Init(uint16_t _cr1);



void bsp_SetSpiSck(uint8_t _data);


