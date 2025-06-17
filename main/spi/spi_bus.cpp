/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动类
*	文件名称 : spi_bus.cpp
*	版    本 : V1.0
*	说    明 : SPI总线类的实现文件
*
*********************************************************************************************************
*/

#include "spi_bus.h"

/*
	PB3/SPI1_SCK
	PB4/SPI1_MISO
	PB5/SPI1_MOSI

	SPI1的时钟源是 APB2Periph
*/

/* 定义SPI总线的 GPIO端口 */
#define RCC_SCK     RCC_APB2Periph_GPIOB
#define PORT_SCK    GPIOB
#define PIN_SCK     GPIO_Pin_3

#define RCC_MISO    RCC_APB2Periph_GPIOB
#define PORT_MISO   GPIOB
#define PIN_MISO    GPIO_Pin_4

#define RCC_MOSI    RCC_APB2Periph_GPIOB
#define PORT_MOSI   GPIOB
#define PIN_MOSI    GPIO_Pin_5

#ifdef SOFT_SPI
    #define SCK_0()     PORT_SCK->BRR = PIN_SCK
    #define SCK_1()     PORT_SCK->BSRR = PIN_SCK

    #define MOSI_0()    PORT_MOSI->BRR = PIN_MOSI
    #define MOSI_1()    PORT_MOSI->BSRR = PIN_MOSI

    #define MISO_IS_HIGH()  (GPIO_ReadInputDataBit(PORT_MISO, PIN_MISO) == Bit_SET)
#endif

#ifdef HARD_SPI
    #define SPI_HARD    SPI1
    #define RCC_SPI     RCC_APB2Periph_SPI1
    
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

// 静态成员初始化
__IO uint32_t SPIBus::timeout = SPIT_LONG_TIMEOUT;
uint8_t SPIBus::busy = 0;

/**
 * @brief 初始化SPI总线
 */
void SPIBus::Init() {
#ifdef SOFT_SPI
    InitSoftwareSPI();
#else
    InitHardwareSPI();
#endif
}

/**
 * @brief 初始化硬件SPI
 */
void SPIBus::InitHardwareSPI() {
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_SPI, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_SCK | RCC_MOSI | RCC_MISO, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = PIN_SCK;
    GPIO_Init(PORT_SCK, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PIN_MOSI;
    GPIO_Init(PORT_MOSI, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PIN_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(PORT_MISO, &GPIO_InitStructure);

    // 配置SPI工作模式
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI_HARD, &SPI_InitStructure);

    SPI_Cmd(SPI_HARD, ENABLE); // 开启SPI
}

/**
 * @brief 初始化软件SPI
 */
void SPIBus::InitSoftwareSPI() {
    GPIO_InitTypeDef GPIO_InitStructure;
    // 打开GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_SCK | RCC_MOSI | RCC_MISO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    //  配置SPI引脚SCK、MISO 和 MOSI为复用推挽模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出模式
    GPIO_InitStructure.GPIO_Pin = PIN_SCK;
    GPIO_Init(PORT_SCK, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PIN_MOSI;
    GPIO_Init(PORT_MOSI, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 配置MISO为输入上拉
    GPIO_InitStructure.GPIO_Pin = PIN_MISO;
    GPIO_Init(PORT_MISO, &GPIO_InitStructure);
}

/**
 * @brief 配置SPI工作模式
 * @param cr1 配置参数
 */
void SPIBus::Configure(uint16_t cr1) {
#ifdef HARD_SPI
    SPI_HARD->CR1 = ((SPI_HARD->CR1 & CR1_CLEAR_Mask) | cr1);
    SPI_HARD->CR1 &= CR1_SPE_Reset;
    SPI_HARD->CR1 |= CR1_SPE_Set;
#endif
}

/**
 * @brief 发送一个字节（CPOL=0, CPHA=0）
 * @param byte 要发送的字节
 * @return 发送成功返回0，否则返回错误码
 */
uint8_t SPIBus::Write0(uint8_t byte) {
#ifdef SOFT_SPI
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if (byte & 0x80) {
            MOSI_1();
        } else {
            MOSI_0();
        }
        SCK_0();
        byte <<= 1;
        for (volatile int j = 0; j < 20; j++)
            ;
        SCK_1();
        for (volatile int j = 0; j < 20; j++)
            ;
    }
    return 0;
#else
    timeout = SPIT_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_TXE) == RESET) {
        if ((timeout--) == 0)
            return TimeoutCallback(0);
    }
    SPI_I2S_SendData(SPI_HARD, byte);

    timeout = SPIT_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_RXNE) == RESET) {
        if ((timeout--) == 0)
            return TimeoutCallback(1);
    }
    return SPI_I2S_ReceiveData(SPI_HARD);
#endif
}

/**
 * @brief 读取一个字节（CPOL=0, CPHA=0）
 * @return 读取到的字节
 */
uint8_t SPIBus::Read0() {
#ifdef SOFT_SPI
    uint8_t i;
    uint8_t read = 0;
    for (i = 0; i < 8; i++) {
        read = read << 1;
        if (MISO_IS_HIGH()) {
            read++;
        }
        SCK_0();
        for (volatile int j = 0; j < 20; j++)
            ;
        SCK_1();
        for (volatile int j = 0; j < 20; j++)
            ;
    }
    return read;
#else
    timeout = SPIT_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_TXE) == RESET) {
        if ((timeout--) == 0)
            return TimeoutCallback(0);
    }
    SPI_I2S_SendData(SPI_HARD, Dummy_Byte);

    timeout = SPIT_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_RXNE) == RESET) {
        if ((timeout--) == 0)
            return TimeoutCallback(1);
    }
    return SPI_I2S_ReceiveData(SPI_HARD);
#endif
}

/**
 * @brief 发送一个字节（CPOL=1, CPHA=0）
 * @param byte 要发送的字节
 * @return 发送成功返回0，否则返回错误码
 */
uint8_t SPIBus::Write1(uint8_t byte) {
#ifdef SOFT_SPI
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if (byte & 0x80) {
            MOSI_1();
        } else {
            MOSI_0();
        }
        SCK_1();
        byte <<= 1;
        for (volatile int j = 0; j < 20; j++)
            ;
        SCK_0();
        for (volatile int j = 0; j < 20; j++)
            ;
    }
    return 0;
#else
    timeout = SPIT_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_TXE) == RESET) {
        if ((timeout--) == 0)
            return TimeoutCallback(0);
    }
    SPI_I2S_SendData(SPI_HARD, byte);

    timeout = SPIT_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_RXNE) == RESET) {
        if ((timeout--) == 0)
            return TimeoutCallback(1);
    }
    return SPI_I2S_ReceiveData(SPI_HARD);
#endif
}

/**
 * @brief 读取一个字节（CPOL=1, CPHA=0）
 * @return 读取到的字节
 */
uint8_t SPIBus::Read1() {
#ifdef SOFT_SPI
    uint8_t i;
    uint8_t read = 0;
    for (i = 0; i < 8; i++) {
        SCK_1();
        for (volatile int j = 0; j < 20; j++)
            ;
        read = read << 1;
        if (MISO_IS_HIGH()) {
            read++;
        }
        SCK_0();
        for (volatile int j = 0; j < 20; j++)
            ;
    }
    return read;
#else
    timeout = SPIT_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_TXE) == RESET) {
        if ((timeout--) == 0)
            return TimeoutCallback(0);
    }
    SPI_I2S_SendData(SPI_HARD, Dummy_Byte);

    timeout = SPIT_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI_HARD, SPI_I2S_FLAG_RXNE) == RESET) {
        if ((timeout--) == 0)
            return TimeoutCallback(1);
    }
    return SPI_I2S_ReceiveData(SPI_HARD);
#endif
}

/**
 * @brief 设置SCK状态
 * @param data 状态
 */
void SPIBus::SetSck(uint8_t data) {
#ifdef SOFT_SPI
    if (data == 0) {
        SCK_0();
    } else {
        SCK_1();
    }
#endif
}

/**
 * @brief 超时回调函数
 * @param errorCode 错误码
 * @return 0
 */
uint16_t SPIBus::TimeoutCallback(uint8_t errorCode) {
    return 0;
}