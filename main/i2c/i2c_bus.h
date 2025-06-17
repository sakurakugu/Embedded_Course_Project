/*
*********************************************************************************************************
*
*	模块名称 : I2C总线驱动类
*	文件名称 : i2c_bus.h
*	版    本 : V1.0
*	说    明 : I2C总线基类的头文件
*
*********************************************************************************************************
*/

#pragma once

#include "stm32f10x.h"
#include <inttypes.h>

#define I2C_WR    0       /* 写控制bit */
#define I2C_RD    1       /* 读控制bit */

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C    GPIOB           /* GPIO端口 */
#define RCC_I2C_PORT     RCC_APB2Periph_GPIOB        /* GPIO端口时钟 */
#define I2C_SCL_PIN      GPIO_Pin_10         /* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN      GPIO_Pin_11         /* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1    /* 条件编译： 1 选择GPIO的库函数实现IO读写 */
    #define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)        /* SCL = 1 */
    #define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)      /* SCL = 0 */
    
    #define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)        /* SDA = 1 */
    #define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)      /* SDA = 0 */
    
    #define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)    /* 读SDA口线状态 */
#else    /* 这个分支选择直接寄存器操作实现IO读写 */
    #define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN              /* SCL = 1 */
    #define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN               /* SCL = 0 */
    
    #define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN              /* SDA = 1 */
    #define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN               /* SDA = 0 */
    
    #define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)    /* 读SDA口线状态 */
#endif

class I2CBus {
  public:
    // 单例模式
    static I2CBus &GetInstance() {
        static I2CBus instance;
        return instance;
    }
    I2CBus(const I2CBus &) = delete;
    I2CBus &operator=(const I2CBus &) = delete;

    void Init();                          // 初始化I2C总线
    uint8_t CheckDevice(uint8_t address); // 检查设备是否存在
    void SendByte(uint8_t byte);          // 发送一个字节
    uint8_t ReadByte();                   // 读取一个字节
    uint8_t WaitAck();                    // 等待ACK
    void SendAck();                       // 发送ACK
    void SendNAck();                      // 发送NACK
    void Start();                         // 发送起始信号
    void Stop();                          // 发送停止信号

  protected:
    I2CBus() = default; // 保护构造函数
    ~I2CBus() = default;

  private:
    static void Delay();         // 延时函数
    void ConfigureGPIO(); // 配置GPIO
};