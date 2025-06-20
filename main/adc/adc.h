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
#define    NTC_ADC_PIN                   GPIO_Pin_0
// ADC 通道宏定义
#define    RV_ADC_CHANNEL                ADC_Channel_14
#define    NTC_ADC_CHANNEL               ADC_Channel_10
// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler
class ADC {
public:
    // 单例模式
    static ADC& GetInstance() {
        static ADC instance;
        return instance;
    }
    // 删除拷贝构造和赋值操作
    ADC(const ADC&) = delete;
    ADC& operator=(const ADC&) = delete;

    void Init(); // ADC 初始化
    uint16_t GetValue(uint8_t channel);// 获取ADC值

private:
    ADC() = default; // 私有构造函数
    ~ADC() = default; // 私有析构函数

    static void GPIO_Config(); // ADC GPIO 初始化
    static void Mode_Config(uint8_t channel); // ADC 模式配置
    static void NVIC_Config(); // ADC 中断配置
};

#endif /* __ADC_H */

