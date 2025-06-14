#ifndef __EXTI_H
#define	__EXTI_H


#include "bsp.h"

#ifdef	proteus
//引脚定义
#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_8
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource8
#define KEY1_INT_EXTI_LINE         EXTI_Line8
#define KEY1_INT_EXTI_IRQ          EXTI9_5_IRQn

#define KEY1_IRQHandler            EXTI9_5_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOA
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_10
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource10
#define KEY2_INT_EXTI_LINE         EXTI_Line10
#define KEY2_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY2_IRQHandler            EXTI15_10_IRQHandler

#else
//引脚定义
#define KEY1_INT_GPIO_PORT         GPIOD
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_8
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource8
#define KEY1_INT_EXTI_LINE         EXTI_Line8
#define KEY1_INT_EXTI_IRQ          EXTI9_5_IRQn

#define KEY1_IRQHandler            EXTI9_5_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOD
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_10
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource10
#define KEY2_INT_EXTI_LINE         EXTI_Line10
#define KEY2_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY2_IRQHandler            EXTI15_10_IRQHandler

#endif

void EXTI_Key_Config(void);


#endif /* __EXTI_H */
