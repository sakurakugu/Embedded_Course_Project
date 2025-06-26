#include "bsp_GeneralTim.h"

static void GENERAL_TIM_GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 输出比较通道1 GPIO 初始化
    //	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
    //  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
    //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);

    // 输出比较通道2 GPIO 初始化
    RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(GENERAL_TIM_CH2_PORT, GENERAL_TIM_CH2_PIN); // 输出0

    // 输出比较通道3 GPIO 初始化
    RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(GENERAL_TIM_CH3_PORT, GENERAL_TIM_CH3_PIN); // 输出0

    // 输出比较通道4 GPIO 初始化
    RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef;
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void) {
    // 开启定时器时钟,即内部时钟CK_INT=72M
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);

    /*--------------------时基结构体初始化-------------------------*/
    // 配置周期，这里配置为100K

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_Period; // 对应ARR
    // 驱动CNT计数器的时钟 = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM_Prescaler;
    // 时钟分频因子 ，配置死区时间时需要用到
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // 重复计数器的值，没用到不用管
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // 初始化定时器
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

    /*--------------------输出比较结构体初始化-------------------*/

    TIM_OCInitTypeDef TIM_OCInitStructure;
    // 配置为PWM模式1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // 输出使能
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // 输出通道电平极性配置
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 输出比较通道 1
    //	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
    //	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
    //	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);

    // 输出比较通道 2
    TIM_OCInitStructure.TIM_Pulse = 200; // 对应CCR
    TIM_OC2Init(GENERAL_TIM, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);

    // 输出比较通道 3
    TIM_OCInitStructure.TIM_Pulse = 200;
    TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);

    // 输出比较通道 4
    TIM_OCInitStructure.TIM_Pulse = 125;
    TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);

    // 允许在定时器工作时向ARR缓冲器写入新值，并在更新事件发生时将新值载入当前寄存器
    TIM_ARRPreloadConfig(GENERAL_TIM, ENABLE);
    // 使能计数器
    TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM_Init(void) {
    GENERAL_TIM_GPIO_Config();
    GENERAL_TIM_Mode_Config();
    TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Disable);
    TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable);
    TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Disable);
}

void DCMotor_Dir_Config(uint8_t Dir) {
    GPIO_InitTypeDef GPIO_InitStructure;

    if (Dir == 1) {
        // RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK, ENABLE);
        TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable); // 禁用通道3输出
        // 输出比较通道3 GPIO 初始化
        // RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH3_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
        GPIO_ResetBits(GENERAL_TIM_CH3_PORT, GENERAL_TIM_CH3_PIN); // 输出0

        GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH2_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);
        TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Enable); // 使能通道2输出
        // 输出比较通道2 GPIO 初始化
    } else {
        TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Disable); // 禁用通道2输出
        // 输出比较通道2 GPIO 初始化
        GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH2_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);
        GPIO_ResetBits(GENERAL_TIM_CH2_PORT, GENERAL_TIM_CH2_PIN); // 输出0

        // 输出比较通道3 GPIO 初始化
        // RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GENERAL_TIM_CH3_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
        TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable); // 使能通道3输出
    }
}

/*********************************************END OF FILE**********************/
