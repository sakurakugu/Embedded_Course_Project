#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f10x.h"


/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM4

#define            GENERAL_TIM                   TIM4
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM4
#define            GENERAL_TIM_Period            255	//us
#define            GENERAL_TIM_Prescaler         71		//1us


// TIM4 输出比较通道2
#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOD
#define            GENERAL_TIM_CH2_PORT          GPIOD
#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_13

// TIM4 输出比较通道3
#define            GENERAL_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOD
#define            GENERAL_TIM_CH3_PORT          GPIOD
#define            GENERAL_TIM_CH3_PIN           GPIO_Pin_14

// TIM4 输出比较通道4
#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOD
#define            GENERAL_TIM_CH4_PORT          GPIOD
#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_15

/**************************函数声明********************************/

void GENERAL_TIM_Init(void);
void DCMotor_Dir_Config(uint8_t Dir);

#endif	/* __BSP_GENERALTIME_H */


