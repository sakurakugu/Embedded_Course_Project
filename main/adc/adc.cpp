#include "adc.h"

__IO uint16_t ADC_ConvertedValue;

/**
 * @brief  ADC初始化
 * @param  无
 * @retval 无
 */
void ADC::Init() {
    GPIO_Config();
    // Mode_Config(ADC_CHANNEL);
    // NVIC_Config();
}

/**
 * @brief  ADC GPIO 初始化
 * @param  无
 * @retval 无
 */
void ADC::GPIO_Config() {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 打开 ADC IO端口时钟
    ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE);

    // 配置 ADC IO 引脚模式
    // 必须为模拟输入
    GPIO_InitStructure.GPIO_Pin = RV_ADC_PIN | NTC_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

    // 初始化 ADC IO
    GPIO_Init(ADC_PORT, &GPIO_InitStructure);
}

/**
 * @brief  配置ADC工作模式
 * @param  无
 * @retval 无
 */
void ADC::Mode_Config(uint8_t channel) {
    ADC_InitTypeDef ADC_InitStructure;

    // 打开ADC时钟
    ADC_APBxClock_FUN(ADC_CLK, ENABLE);

    ADC_DeInit(ADCx); // 复位ADCx
    // ADC 模式配置
    // 只使用一个ADC，属于独立模式
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 禁止扫描模式，多通道才要，单通道不需要
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 不用外部触发转换，软件开启即可
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 转换结果右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1; // 转换通道1个
    ADC_Init(ADCx, &ADC_InitStructure); // 初始化ADC
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); // 配置ADC时钟为PCLK2的8分频，即9MHz

    // 配置 ADC 通道转换顺序和采样时间

    // ADC 转换结束产生中断，在中断服务程序中读取转换值
    // ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
    ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime_55Cycles5);

    ADC_Cmd(ADCx, ENABLE); // 开启ADC ，并开始转换
    ADC_ResetCalibration(ADCx); // 初始化ADC 校准寄存器
    while (ADC_GetResetCalibrationStatus(ADCx))
        ; // 等待校准寄存器初始化完成
    ADC_StartCalibration(ADCx); // ADC开始校准
    while (ADC_GetCalibrationStatus(ADCx))
        ; // 等待校准完成

    ADC_SoftwareStartConvCmd(ADCx, ENABLE); // 由于没有采用外部触发，所以使用软件触发ADC转换
}

void ADC::NVIC_Config() {
    NVIC_InitTypeDef NVIC_InitStructure;
    // 优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    // 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

uint16_t ADC::GetValue(uint8_t channel) {
    Mode_Config(channel);
    while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC)) {
    }; // 等待转换完成
    return ADC_GetConversionValue(ADCx); // 返回12位ADC值
}
/*********************************************END OF FILE**********************/
