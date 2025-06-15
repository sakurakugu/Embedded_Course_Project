/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*
*********************************************************************************************************
*/

#ifndef __BSP_H
#define __BSP_H

/*
	stm32f10x.h :STM32F10x系列CPU的标准外设固件库。这个固件包含了很多型号的CPU，
		使用这个库必须在工程中添加预编译的宏:
			USE_STDPERIPH_DRIVER  表示启用标准外设库
			STM32F10X_HD          表示CPU是10x大容量版本(HD = High Density)
	
	stdlib.h : standard library标准库头文件,包含了C、C++语言的最常用的类型定义和系统函数，比如 malloc()、free()
	
	stdio.h : standard input & output 标准输入输出函数库。常用函数有 printf()、getc()、putc()
	
	string.h : 关于字符数组的函数定义的头文件库，常用函数有strlen、strcmp、strcpy
	
	" " 和 < > 的区别:
	" " 一般表示源代码自带的h文件。编译时，编译器优先在工程配置指定的搜索路径列表中寻找头文件
	< > 一般表示编译系统自带的h文件。编译时，编译器优先在IDE自带的库文件夹寻找头文件。
*/
#include "stm32f10x.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gpio_led.h"
#include "bsp_key.h"
#include "bsp_spi_bus.h"
#include "stm32f10x_spi.h"

#include "bsp_segdz.h"
#include "bsp_usart.h"
#include "bsp_JQ8900.h"
#include "bsp_ESPxx.h"
#include "bsp_tlink.h"

#include "bsp_i2c_gpio.h"
#include "bsp_i2c_ee.h"
#include "bsp_i2c_lm75.h"

#include "bsp_oled.h"
#include "oledFont.h"

#include "bsp_lcd.h"
#include "lcdFont.h" 
#include "bsp_gui.h"

#include "touch.h" 

#include "bsp_adc.h"
#include "bsp_GeneralTim.h" 
#include "bsp_AdvanceTim.h" 

#include "stm32f10x_it.h"
#include "misc.h"
#include "bsp_exti.h"
#include "stm32f10x_rcc.h" 
#include "stm32f10x_flash.h"
#include "stm32f10x_usart.h"

#include "bsp_SysTick.h"
#include "core_cm3.h"
#include "misc.h"




//#define	systick_demo
//#define	seg_dz_demo
//#define	key_demo
//#define exti_key_demo

//#define proteus
extern uint8_t	bsp_RunPer1ms;
extern uint8_t	bsp_RunPer10ms;

extern uint8_t	bsp_RunPer1s;

extern uint32_t MsCount;

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);

void Delay(__IO uint32_t nCount);

void DelayUs(uint32_t nCount);
void DelayMs(uint32_t nCount);
	
void HSI_SetSysClock(uint32_t pllmul);

void dispdata(uint16_t count);
void Show_Message(void);

void test(uint8_t ch);

#endif

