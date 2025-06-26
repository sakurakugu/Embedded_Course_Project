#pragma once

// 数码管 和 点阵 的 IO 定义
#define DZ595_OE_PORT  GPIOC
#define DZ595_OE_PIN   GPIO_Pin_9 
#define DZ595_ST_PORT  GPIOC
#define DZ595_ST_PIN   GPIO_Pin_11

#define SEG595_OE_PORT  GPIOC
#define SEG595_OE_PIN   GPIO_Pin_10
#define SEG595_ST_PORT  GPIOC
#define SEG595_ST_PIN   GPIO_Pin_12

#define DISP_EN_PORT  GPIOC
#define DISP_EN_PIN   GPIO_Pin_1

#define DISP_SEL_PORT  GPIOB
#define DISP_SEL_0  	GPIO_Pin_12
#define DISP_SEL_1  	GPIO_Pin_13
#define DISP_SEL_2  	GPIO_Pin_14
#define DISP_SEL_3  	GPIO_Pin_15

// 按键 的 IO 定义
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA		   
#define    KEY1_GPIO_PIN		 GPIO_Pin_8

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY2_GPIO_PORT    GPIOA	   
#define    KEY2_GPIO_PIN		  GPIO_Pin_9

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY3_GPIO_PORT    GPIOA		   
#define    KEY3_GPIO_PIN		  GPIO_Pin_10

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY4_GPIO_PORT    GPIOA		   
#define    KEY4_GPIO_PIN		  GPIO_Pin_11

#define    KEYL_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEYL_GPIO_PORT    GPIOB			   
#define    KEYL1_GPIO_PIN		 GPIO_Pin_0

#define    KEYL2_GPIO_PIN		 GPIO_Pin_1
#define    KEYL3_GPIO_PIN		 GPIO_Pin_8

// 按键 的 IO 定义
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