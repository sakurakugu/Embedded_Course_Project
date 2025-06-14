#ifndef __KEY_H
#define	__KEY_H


#include "bsp.h"

//  引脚定义
#ifdef proteus
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
#else
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY1_GPIO_PORT    GPIOD			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_8

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY2_GPIO_PORT    GPIOD		   
#define    KEY2_GPIO_PIN		  GPIO_Pin_9

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY3_GPIO_PORT    GPIOD		   
#define    KEY3_GPIO_PIN		  GPIO_Pin_10

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY4_GPIO_PORT    GPIOD		   
#define    KEY4_GPIO_PIN		  GPIO_Pin_11

#define    KEYL_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEYL_GPIO_PORT    GPIOB			   
#define    KEYL1_GPIO_PIN		 GPIO_Pin_15

#define    KEYL2_GPIO_PIN		 GPIO_Pin_14

#define    KEYL3_GPIO_PIN		 GPIO_Pin_13

#endif


 /** 按键按下标置宏
	*  按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	*  若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	0
#define KEY_OFF	1

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void Key_Com_GPIO_Config(void);

uint8_t read_key(void);


#endif /* __KEY_H */

