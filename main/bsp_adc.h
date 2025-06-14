#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_PORT                      GPIOC
#define    RV_ADC_PIN                    GPIO_Pin_4
#define    NTC_ADC_PIN                    GPIO_Pin_0
// ADC 通道宏定义
#define    RV_ADC_CHANNEL                   ADC_Channel_14
#define    NTC_ADC_CHANNEL                  ADC_Channel_10

// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

//#define    ADC_IRQ                       ADC3_IRQn
//#define    ADC_IRQHandler                ADC3_IRQHandler


void ADCx_Init(void);
uint16_t getAdc(uint8_t ADC_CHANNEL_X);

#endif /* __ADC_H */

