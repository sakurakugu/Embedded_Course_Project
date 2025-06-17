/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动类
*	文件名称 : spi_bus.h
*	版    本 : V1.0
*	说    明 : SPI总线类的头文件
*
*********************************************************************************************************
*/

#pragma once

#include "stm32f10x.h"
#include <cstdint>

//#define SOFT_SPI		/* 定义此行表示使用GPIO模拟SPI接口 */
#define HARD_SPI		/* 定义此行表示使用CPU的硬件SPI接口 */

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

#define Dummy_Byte 0x00

class SPIBus {
  public:
    // 单例模式
    static SPIBus &GetInstance() {
        static SPIBus instance;
        return instance;
    }
    // 禁止拷贝和赋值
    SPIBus(const SPIBus &) = delete;
    SPIBus &operator=(const SPIBus &) = delete;

    void Init();                  // 初始化SPI总线
    void Configure(uint16_t cr1); // 配置SPI工作模式
    uint8_t Write0(uint8_t byte); // 发送一个字节 (CPOL=0, CPHA=0)
    uint8_t Read0();              // 接收一个字节 (CPOL=0, CPHA=0)
    uint8_t Write1(uint8_t byte); // 发送一个字节 (CPOL=1, CPHA=0)
    uint8_t Read1();              // 接收一个字节 (CPOL=1, CPHA=0)
    void SetSck(uint8_t data);    // 设置SCK状态（仅软件SPI模式）

  private:
    SPIBus() = default; // 私有构造函数
    ~SPIBus() = default;

    static uint16_t TimeoutCallback(uint8_t errorCode); // 超时回调函数
    void InitHardwareSPI(); // 初始化硬件SPI
    void InitSoftwareSPI(); // 初始化软件SPI

    static __IO uint32_t timeout;
    static uint8_t busy;
};

