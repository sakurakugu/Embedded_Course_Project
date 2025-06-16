#pragma once

#include "stm32f10x.h"
#include <stdint.h>

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
// R-红色
#define LED1_GPIO_PORT    	GPIOC		                /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define LED1_GPIO_PIN		GPIO_Pin_7			        /* 连接到SCL时钟线的GPIO */
// G-绿色
#define LED2_GPIO_PORT    	GPIOC			            /* GPIO端口 */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define LED2_GPIO_PIN		GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */
// B-蓝色
#define LED3_GPIO_PORT    	GPIOC			            /* GPIO端口 */
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define LED3_GPIO_PIN		GPIO_Pin_6			        /* 连接到SCL时钟线的GPIO */

//  按键的引脚定义
#ifdef proteus // Proteus仿真板
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA		   
#define    KEY1_GPIO_PIN	 GPIO_Pin_8

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY2_GPIO_PORT    GPIOA	   
#define    KEY2_GPIO_PIN	 GPIO_Pin_9

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY3_GPIO_PORT    GPIOA		   
#define    KEY3_GPIO_PIN	 GPIO_Pin_10

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY4_GPIO_PORT    GPIOA		   
#define    KEY4_GPIO_PIN	 GPIO_Pin_11

#define    KEYL_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEYL_GPIO_PORT    GPIOB			   
#define    KEYL1_GPIO_PIN	 GPIO_Pin_0

#define    KEYL2_GPIO_PIN	 GPIO_Pin_1
#define    KEYL3_GPIO_PIN	 GPIO_Pin_8
#else // STM32F103VCT6开发板
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY1_GPIO_PORT    GPIOD			   
#define    KEY1_GPIO_PIN	 GPIO_Pin_8

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY2_GPIO_PORT    GPIOD		   
#define    KEY2_GPIO_PIN	 GPIO_Pin_9

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY3_GPIO_PORT    GPIOD		   
#define    KEY3_GPIO_PIN	 GPIO_Pin_10

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY4_GPIO_PORT    GPIOD		   
#define    KEY4_GPIO_PIN	 GPIO_Pin_11

#define    KEYL_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEYL_GPIO_PORT    GPIOB			   
#define    KEYL1_GPIO_PIN	 GPIO_Pin_15

#define    KEYL2_GPIO_PIN	 GPIO_Pin_14

#define    KEYL3_GPIO_PIN	 GPIO_Pin_13

#endif