#pragma once

#include "stm32f10x.h"

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