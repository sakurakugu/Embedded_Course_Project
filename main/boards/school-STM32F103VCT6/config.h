#pragma once

// LCD 的 IO 定义
#define USE_HORIZONTAL 0 // 液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转
#define LCD_W 320 // LCD宽度
#define LCD_H 480 // LCD高度

// 数码管 和 点阵 的 IO 定义
#define DZ595_OE_PORT  GPIOE
#define DZ595_OE_PIN   GPIO_Pin_9 
#define DZ595_ST_PORT  GPIOE
#define DZ595_ST_PIN   GPIO_Pin_11

#define SEG595_OE_PORT  GPIOE
#define SEG595_OE_PIN   GPIO_Pin_10

#define SEG595_ST_PORT  GPIOD
#define SEG595_ST_PIN   GPIO_Pin_1

#define DISP_EN_PORT  GPIOE
#define DISP_EN_PIN   GPIO_Pin_1

#define DISP_SEL_PORT  GPIOE
#define DISP_SEL_0  	GPIO_Pin_12
#define DISP_SEL_1  	GPIO_Pin_13
#define DISP_SEL_2  	GPIO_Pin_14
#define DISP_SEL_3  	GPIO_Pin_15

// OLED 的 IO 定义
#define OLED_SIZE   16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 	 
#define OLED_W   	128
#define OLED_H		64		
#define GRAM_H 		OLED_H/8

// I2C 的 IO 定义
#define GPIO_PORT_I2C	GPIOB			      // GPIO端口 
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB  // GPIO端口时钟 
#define I2C_SCL_PIN		GPIO_Pin_10			  // 连接到SCL时钟线的GPIO 
#define I2C_SDA_PIN		GPIO_Pin_11			  // 连接到SDA数据线的GPIO 

// 按键 的 IO 定义
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

// LED 的 IO 定义
// R-红色
#define LED1_GPIO_PORT    	GPIOC		              /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define LED1_GPIO_PIN		GPIO_Pin_7			        /* 连接到SCL时钟线的GPIO */

// G-绿色
#define LED2_GPIO_PORT    	GPIOC			              /* GPIO端口 */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define LED2_GPIO_PIN		GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */

// B-蓝色
#define LED3_GPIO_PORT    	GPIOC			              /* GPIO端口 */
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define LED3_GPIO_PIN		GPIO_Pin_6			        /* 连接到SCL时钟线的GPIO */

// 当高电平时LED亮，则LED_ON=0，LED_OFF=1，否则反之
#define LED_ON  0
#define LED_OFF 1

// Tlink 的 IO 定义
#define  Tlink_OK_GPIO_CLK           RCC_APB2Periph_GPIOA
#define  Tlink_OK_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
#define  Tlink_OK_PORT               GPIOA
#define  Tlink_OK_GPIO_PIN           GPIO_Pin_9

// ADC 的 IO 定义
// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx_                          ADC1
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


// 高级定时器 的 IO 定义
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

// ESPxx 串口 的 IO 定义
//串口1-USART1
#define  ESP_USARTx                   USART1
#define  ESP_USART_CLK                RCC_APB2Periph_USART1
#define  ESP_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  ESP_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  ESP_USART_GPIO_CLK           RCC_APB2Periph_GPIOB
#define  ESP_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

#define  UART1_PB6_PB7
#define  ESP_USART_TX_GPIO_PORT         GPIOB   
#define  ESP_USART_TX_GPIO_PIN          GPIO_Pin_6
#define  ESP_USART_RX_GPIO_PORT       GPIOB
#define  ESP_USART_RX_GPIO_PIN        GPIO_Pin_7

#define  ESP_EN_GPIO_CLK           RCC_APB2Periph_GPIOE
#define  ESP_EN_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
#define  ESP_EN_PORT       			GPIOE
#define  ESP_EN_GPIO_PIN        GPIO_Pin_3

#define  ESP_USART_IRQ                USART1_IRQn
#define  ESP_USART_IRQHandler         USART1_IRQHandler


// 按键 的 IO 定义
#define KEY1_INT_GPIO_PORT         GPIOD
#define KEY1_INT_GPIO_CLK          RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO
#define KEY1_INT_GPIO_PIN          GPIO_Pin_8
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource8
#define KEY1_INT_EXTI_LINE         EXTI_Line8
#define KEY1_INT_EXTI_IRQ          EXTI9_5_IRQn

#define KEY1_IRQHandler            EXTI9_5_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOD
#define KEY2_INT_GPIO_CLK          RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO
#define KEY2_INT_GPIO_PIN          GPIO_Pin_10
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource10
#define KEY2_INT_EXTI_LINE         EXTI_Line10
#define KEY2_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY2_IRQHandler            EXTI15_10_IRQHandler

// 通用定时器 的 IO 定义
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


// JQ8900 串口 的 IO 定义
// 串口5-UART5
#define  JQ8900_USARTx                   UART5
#define  JQ8900_USART_CLK                RCC_APB1Periph_UART5
#define  JQ8900_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  JQ8900_USART_BAUDRATE           9600

// USART GPIO 引脚宏定义
#define  JQ8900_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  JQ8900_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  JQ8900_USART_TX_GPIO_PORT         GPIOC   
#define  JQ8900_USART_TX_GPIO_PIN          GPIO_Pin_12
#define  JQ8900_USART_RX_GPIO_PORT       GPIOD
#define  JQ8900_USART_RX_GPIO_PIN        GPIO_Pin_2

#define  JQ8900_USART_IRQ                UART5_IRQn
#define  JQ8900_USART_IRQHandler         UART5_IRQHandler


// USART 串口 的 IO 定义
// 串口2-USART2
#define  DEBUG_USARTx                   USART2
#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOD)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
 
#define  UART2_PD5_PD6 
#define  DEBUG_USART_TX_GPIO_PORT         GPIOD  
#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_5
#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_6

#define  DEBUG_USART_IRQ                USART2_IRQn
#define  DEBUG_USART_IRQHandler         USART2_IRQHandler

