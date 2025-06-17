#pragma once

#include "stm32f10x.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
// R-红色
#define    LED1_GPIO_PORT    GPIOC		                /* GPIO端口 */
#define    LED1_GPIO_CLK 	 RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define    LED1_GPIO_PIN	 GPIO_Pin_7			        /* 连接到SCL时钟线的GPIO */
// G-绿色
#define    LED2_GPIO_PORT    GPIOC			            /* GPIO端口 */
#define    LED2_GPIO_CLK 	 RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define    LED2_GPIO_PIN	 GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */
// B-蓝色
#define    LED3_GPIO_PORT    GPIOC			            /* GPIO端口 */
#define    LED3_GPIO_CLK 	 RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define    LED3_GPIO_PIN     GPIO_Pin_6			        /* 连接到SCL时钟线的GPIO */

// 按键的引脚定义
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

// 数码管和点阵显示控制类的GPIO定义
#define    DZ595_OE_PORT     GPIOE
#define    DZ595_OE_PIN      GPIO_Pin_9 
#define    DZ595_ST_PORT     GPIOE
#define    DZ595_ST_PIN      GPIO_Pin_11

#define    SEG595_OE_PORT    GPIOE
#define    SEG595_OE_PIN     GPIO_Pin_10

#define    SEG595_ST_PORT    GPIOD
#define    SEG595_ST_PIN     GPIO_Pin_1

#define    DISP_EN_PORT      GPIOE
#define    DISP_EN_PIN       GPIO_Pin_1

#define    DISP_SEL_PORT     GPIOE
#define    DISP_SEL_0  	     GPIO_Pin_12
#define    DISP_SEL_1  	     GPIO_Pin_13
#define    DISP_SEL_2        GPIO_Pin_14
#define    DISP_SEL_3        GPIO_Pin_15

/************高级定时器TIM参数定义，只限TIM1和TIM8************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM1

#define            ADVANCE_TIM                   TIM8
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM8
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (255-1)
#define            ADVANCE_TIM_PSC               (SystemCoreClock / 100000 -1)	//10us
#define            ADVANCE_TIM_PULSE             4		//占空比

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler
// TIM1 输出比较通道
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH1_PORT          GPIOC
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_6
// TIM1 输出比较通道
#define            ADVANCE_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH2_PORT          GPIOC
#define            ADVANCE_TIM_CH2_PIN           GPIO_Pin_7
// TIM1 输出比较通道
#define            ADVANCE_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH3_PORT          GPIOC
#define            ADVANCE_TIM_CH3_PIN           GPIO_Pin_8
// TIM1 输出比较通道的互补通道
//#define            ADVANCE_TIM_CH1N_GPIO_CLK      RCC_APB2Periph_GPIOB
//#define            ADVANCE_TIM_CH1N_PORT          GPIOB
//#define            ADVANCE_TIM_CH1N_PIN           GPIO_Pin_13
// TIM1 输出比较通道的刹车通道
//#define            ADVANCE_TIM_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOB
//#define            ADVANCE_TIM_BKIN_PORT          GPIOB
//#define            ADVANCE_TIM_BKIN_PIN           GPIO_Pin_12