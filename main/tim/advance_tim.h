#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H


#include "stm32f10x.h"


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

/**************************函数声明********************************/

void ADVANCE_TIM_Init(void);


#endif	/* __BSP_ADVANCETIME_H */


