#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;


/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
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
static void ADCx_Mode_Config(uint8_t ADC_CHANNEL_X)
{
	ADC_InitTypeDef ADC_InitStructure;	

	// 打开ADC时钟
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	ADC_DeInit(ADCx_);  // 复位ADCx
	// ADC 模式配置
	// 只使用一个ADC，属于独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// 单次转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道1个
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// 初始化ADC
	ADC_Init(ADCx_, &ADC_InitStructure);
	
	// 配置ADC时钟为PCLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置 ADC 通道转换顺序和采样时间
	ADC_RegularChannelConfig(ADCx_, ADC_CHANNEL_X, 1, 
	                         ADC_SampleTime_55Cycles5);
	
	// ADC 转换结束产生中断，在中断服务程序中读取转换值
	//ADC_ITConfig(ADCx_, ADC_IT_EOC, ENABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(ADCx_, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADCx_);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADCx_));
	
	// ADC开始校准
	ADC_StartCalibration(ADCx_);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADCx_));
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADCx_, ENABLE);
}

// static void ADC_NVIC_Config(void) // 没用到，因此没用静态
void ADC_NVIC_Config(void)
{
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


/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	//ADCx_Mode_Config(ADC_CHANNEL);
	//ADC_NVIC_Config();
}


uint16_t getAdc(uint8_t ADC_CHANNEL_X)
{
	ADCx_Mode_Config(ADC_CHANNEL_X);
	while(!ADC_GetFlagStatus(ADCx_, ADC_FLAG_EOC)){};  // 等待转换完成
    return ADC_GetConversionValue(ADCx_);  // 返回12位ADC值
}
/*********************************************END OF FILE**********************/
