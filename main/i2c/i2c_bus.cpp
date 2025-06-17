/*
*********************************************************************************************************
*
*	模块名称 : I2C总线驱动类
*	文件名称 : I2CBus.cpp
*	版    本 : V1.0
*	说    明 : I2C总线基类的实现文件
*
*********************************************************************************************************
*/

#include "i2c_bus.h"

/**
 * @brief 初始化I2C总线
 */
void I2CBus::Init() {
    ConfigureGPIO();
    Stop();  // 给一个停止信号，复位I2C总线上的所有设备到待机模式
}

/**
 * @brief 配置GPIO
 */
void I2CBus::ConfigureGPIO() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);  // 打开GPIO时钟

    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);
}

/**
 * @brief 延时函数
 */
void I2CBus::Delay() {
    uint8_t i;
    /*　
        下面的时间是通过逻辑分析仪测试得到的。
        工作条件：CPU主频72MHz ，MDK编译环境，1级优化

        循环次数为10时，SCL频率 = 205KHz
        循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us
        循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us
    */
    for (i = 0; i < 10; i++);  // 延时约2.5us
}

/**
 * @brief 发送起始信号
 */
void I2CBus::Start() {
    I2C_SDA_1();
    I2C_SCL_1();
    Delay();
    I2C_SDA_0();
    Delay();
    I2C_SCL_0();
    Delay();
}

/**
 * @brief 发送停止信号
 */
void I2CBus::Stop() {
    I2C_SDA_0();
    I2C_SCL_1();
    Delay();
    I2C_SDA_1();
}

/**
 * @brief 发送一个字节
 * @param byte 要发送的字节
 */
void I2CBus::SendByte(uint8_t byte) {
    uint8_t i;

    for (i = 0; i < 8; i++) {
        if (byte & 0x80) {
            I2C_SDA_1();
        } else {
            I2C_SDA_0();
        }
        Delay();
        I2C_SCL_1();
        Delay();
        I2C_SCL_0();
        if (i == 7) {
            I2C_SDA_1();  // 释放总线
        }
        byte <<= 1;
        Delay();
    }
}

/**
 * @brief 读取一个字节
 * @return 返回值 0 表示正确， 返回1表示未探测到
 */
uint8_t I2CBus::ReadByte() {
    uint8_t i;
    uint8_t value = 0;

    for (i = 0; i < 8; i++) {
        value <<= 1;
        I2C_SCL_1();
        Delay();
        if (I2C_SDA_READ()) {
            value++;
        }
        I2C_SCL_0();
        Delay();
    }
    return value;
}

/**
 * @brief 等待ACK
 * @return 返回值 0 表示正确， 返回1表示未探测到
 */
uint8_t I2CBus::WaitAck() {
    uint8_t re;

    I2C_SDA_1();  // CPU释放SDA总线
    Delay();
    I2C_SCL_1();  // CPU驱动SCL = 1, 此时器件会返回ACK应答
    Delay();
    if (I2C_SDA_READ()) {
        re = 1;
    } else {
        re = 0;
    }
    I2C_SCL_0();
    Delay();
    return re;
}

/**
 * @brief 发送ACK
 */
void I2CBus::SendAck() {
    I2C_SDA_0();  // CPU驱动SDA = 0
    Delay();
    I2C_SCL_1();  // CPU产生1个时钟
    Delay();
    I2C_SCL_0();
    Delay();
    I2C_SDA_1();  // CPU释放SDA总线
}

/**
 * @brief 发送NACK
 */
void I2CBus::SendNAck() {
    I2C_SDA_1();  // CPU驱动SDA = 1
    Delay();
    I2C_SCL_1();  // CPU产生1个时钟
    Delay();
    I2C_SCL_0();
    Delay();
}

/**
 * @brief 检测I2C总线设备
 * @param address 设备地址
 * @return 返回值 0 表示正确， 返回1表示未探测到
 */
uint8_t I2CBus::CheckDevice(uint8_t address) {
    uint8_t ucAck;

    Start();  // 发送启动信号
    SendByte(address | I2C_WR);  // 发送设备地址+写控制位
    ucAck = WaitAck();  // 检测设备的ACK应答
    Stop();  // 发送停止信号

    return ucAck;
} 