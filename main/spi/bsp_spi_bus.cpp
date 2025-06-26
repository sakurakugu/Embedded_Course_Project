/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动
*	文件名称 : bsp_spi_bus.h
*	版    本 : 
*	说    明 : SPI总线底层驱动。提供SPI配置、收发数据、多设备共享SPI支持。通过宏切换是硬件SPI还是软件模拟
*	修改记录 :
*
*********************************************************************************************************
*/

#include "bsp_spi_bus.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
/*
	
	PB3/SPI1_SCK
	PB4/SPI1_MISO
	PB5/SPI1_MOSI

	SPI1的时钟源是 APB2Periph
*/

/* 定义SPI总线的 GPIO端口 */
#define RCC_SCK 	RCC_APB2Periph_GPIOB
#define PORT_SCK	GPIOB
#define PIN_SCK		GPIO_Pin_3

#define RCC_MISO 	RCC_APB2Periph_GPIOB
#define PORT_MISO	GPIOB
#define PIN_MISO	GPIO_Pin_4

#define RCC_MOSI 	RCC_APB2Periph_GPIOB
#define PORT_MOSI	GPIOB
#define PIN_MOSI	GPIO_Pin_5

#ifdef SOFT_SPI		/* 软件SPI */
	#define SCK_0()		PORT_SCK->BRR = PIN_SCK
	#define SCK_1()		PORT_SCK->BSRR = PIN_SCK

	#define MOSI_0()	PORT_MOSI->BRR = PIN_MOSI
	#define MOSI_1()	PORT_MOSI->BSRR = PIN_MOSI

	#define MISO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_MISO, PIN_MISO) == Bit_SET)
#endif

#ifdef HARD_SPI
	#define SPI_HARD	SPI1
	#define RCC_SPI		RCC_APB2Periph_SPI1
	//#define SPI_HARD	SPI3
	//#define RCC_SPI		RCC_APB1Periph_SPI3
	
	/* SPI or I2S mode selection masks */
	#define SPI_Mode_Select      ((uint16_t)0xF7FF)
	#define I2S_Mode_Select      ((uint16_t)0x0800) 
	
	/* SPI registers Masks */
	#define CR1_CLEAR_Mask       ((uint16_t)0x3040)
	#define I2SCFGR_CLEAR_Mask   ((uint16_t)0xF040)

	/* SPI SPE mask */
	#define CR1_SPE_Set          ((uint16_t)0x0040)
	#define CR1_SPE_Reset        ((uint16_t)0xFFBF)
#endif

uint8_t g_spi_busy = 0;		/* SPI 总线共享标志 */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPIBus
*	功能说明: 配置SPI总线。 只包括 SCK、 MOSI、 MISO口线的配置。不包括片选CS，也不包括外设芯片特有的INT、BUSY等
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{
#ifdef SOFT_SPI		/* 软件SPI */
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_MOSI | RCC_MISO, ENABLE);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  // in default, PB3 is a jtag PIN, so here disable the JTAG to connect PB3 with SPICLK
	/* 配置SPI引脚SCK、MISO 和 MOSI为复用推挽模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI;
	GPIO_Init(PORT_MOSI, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* MISO 设置为输入上拉 */
	GPIO_InitStructure.GPIO_Pin = PIN_MISO;
	GPIO_Init(PORT_MISO, &GPIO_InitStructure);
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	
#endif

#ifdef HARD_SPI		/* 硬件SPI */
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  // in default, PB3 is a jtag PIN, so here disable the JTAG to connect PB3 with SPICLK
	
		
	GPIO_PinRemapConfig(GPIO_Remap_SPI1 , ENABLE);   //SPI3
	RCC_APB2PeriphClockCmd(RCC_SPI, ENABLE);		 /* 开启 SPI 时钟  SPI3*/
	
	/* 使能 GPIO 时钟 */
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_MOSI | RCC_MISO, ENABLE);	

	/* 配置 SPI引脚SCK、MISO 和 MOSI为复用推挽模式 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin = PIN_SCK;	
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);
	
	

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI;	
	GPIO_Init(PORT_MOSI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_MISO;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(PORT_MISO, &GPIO_InitStructure);
	
	
	/* SPI 模式配置 */
  // OLED芯片 支持SPI模式0，据此设置CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_HARD , &SPI_InitStructure);

  /* 使能 SPI  */
  SPI_Cmd(SPI_HARD , ENABLE);
	
	/*	
	bsp_SPI_Init(SPI_Direction_2Lines_FullDuplex | SPI_Mode_Master | SPI_DataSize_8b
		| SPI_CPOL_Low | SPI_CPHA_1Edge | SPI_NSS_Soft | SPI_BaudRatePrescaler_2 | SPI_FirstBit_MSB);	
	*/
	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	//SPI_HARD->I2SCFGR &= SPI_Mode_Select;		/* 选择SPI模式，不是I2S模式 */

	/*---------------------------- SPIx CRCPOLY Configuration --------------------*/
	/* Write to SPIx CRCPOLY */
	//SPI_HARD->CRCPR = 7;		/* 一般不用 */


	//SPI_Cmd(SPI_HARD, DISABLE);			/* 先禁止SPI  */

	//SPI_Cmd(SPI_HARD, ENABLE);			/* 使能SPI  */
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SPI_Init
*	功能说明: 配置STM32内部SPI硬件的工作模式。 简化库函数，提高执行效率。 仅用于SPI接口间切换。
*	形    参: _cr1 寄存器值
*	返 回 值: 无
*********************************************************************************************************
*/
#ifdef HARD_SPI		/* 硬件SPI */
void bsp_SPI_Init(uint16_t _cr1)
{
	SPI_HARD->CR1 = ((SPI_HARD->CR1 & CR1_CLEAR_Mask) | _cr1);
	  
	//SPI_Cmd(SPI_HARD, DISABLE);			/* 先禁止SPI  */	    
  SPI_HARD->CR1 &= CR1_SPE_Reset;	/* Disable the selected SPI peripheral */

	//SPI_Cmd(SPI_HARD, ENABLE);			/* 使能SPI  */		    
  SPI_HARD->CR1 |= CR1_SPE_Set;	  /* Enable the selected SPI peripheral */
}
#elif def SOFT_SPI
void bsp_SPI_Init(uint16_t _cr1){;}
#endif

#ifdef SOFT_SPI		/* 软件SPI */
/*
*********************************************************************************************************
*	函 数 名: bsp_SpiDelay
*	功能说明: 时序延迟
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiDelay(void)
{
	uint32_t i;

	for (i = 0; i < 20; i++);
}
#endif

/*
*********************************************************************************************************
*	函 数 名: bsp_spiWrite0
*	功能说明: 向SPI总线发送一个字节。SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiWrite0(uint8_t _ucByte)
{
#ifdef SOFT_SPI		/* 软件SPI */
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		bsp_spiDelay();
		SCK_1();
		_ucByte <<= 1;
		bsp_spiDelay();
		SCK_0();
	}
	//bsp_spiDelay();
#endif

#ifdef HARD_SPI		/* 硬件SPI */
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
  }
	/* 发送一个字节 */
	SPI_I2S_SendData(SPI_HARD, _ucByte);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_RXNE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
  }
	/* 读取接收到的数据 */
	return SPI_I2S_ReceiveData(SPI_HARD);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiRead0
*	功能说明: 从SPI总线接收8个bit数据。 SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t bsp_spiRead0(void)
{
#ifdef SOFT_SPI		/* 软件SPI */
	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		read = read<<1;

		if (MISO_IS_HIGH())
		{
			read++;
		}
		SCK_1();
		bsp_spiDelay();
		SCK_0();
		bsp_spiDelay();
	}
	return read;
#endif

#ifdef HARD_SPI		/* 硬件SPI */
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
  }
	/* 发送一个字节 */
	SPI_I2S_SendData(SPI_HARD, Dummy_Byte);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_RXNE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
  }
	/* 读取接收到的数据 */
	return SPI_I2S_ReceiveData(SPI_HARD);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiWrite1
*	功能说明: 向SPI总线发送一个字节。  SCK上升沿采集数据, SCK空闲时为高电平
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiWrite1(uint8_t _ucByte)
{
#ifdef SOFT_SPI		/* 软件SPI */
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		SCK_0();
		_ucByte <<= 1;
		bsp_spiDelay();
		SCK_1();				/* SCK上升沿采集数据, SCK空闲时为高电平 */
		bsp_spiDelay();
	}
#endif

#ifdef HARD_SPI		/* 硬件SPI */
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
  }
	/* 发送一个字节 */
	SPI_I2S_SendData(SPI_HARD, _ucByte);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_RXNE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
  }
	/* 读取接收到的数据 */
	return SPI_I2S_ReceiveData(SPI_HARD);
	
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiRead1
*	功能说明: 从SPI总线接收8个bit数据。  SCK上升沿采集数据, SCK空闲时为高电平
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(void)
{
#ifdef SOFT_SPI		/* 软件SPI */
	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		SCK_0();
		bsp_spiDelay();
		read = read << 1;
		if (MISO_IS_HIGH())
		{
			read++;
		}
		SCK_1();
		bsp_spiDelay();
	}
	return read;
#endif

#ifdef HARD_SPI		/* 硬件SPI */
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
  }
	/* 发送一个字节 */
	SPI_I2S_SendData(SPI_HARD, Dummy_Byte);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_RXNE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
  }
	/* 读取接收到的数据 */
	return SPI_I2S_ReceiveData(SPI_HARD);
#endif
}


/*
*********************************************************************************************************
*	函 数 名: bsp_SetSpiSck
*	功能说明: 用于软件模式。设置SCK GPIO的状态。在函数CS=0之前被调用，用于不同相序的SPI设备间切换。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#ifdef SOFT_SPI		/* 软件SPI */
void bsp_SetSpiSck(uint8_t _data)
{
	if (_data == 0)
	{
		SCK_0();
	}
	else
	{
		SCK_1();
	}
}
#endif

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  printf("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}

