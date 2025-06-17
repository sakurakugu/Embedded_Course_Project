/*
*********************************************************************************************************
*
*	模块名称 : EEPROM类
*	文件名称 : eeprom.h
*	版    本 : V1.0
*	说    明 : EEPROM类的头文件
*
*********************************************************************************************************
*/

#pragma once

#include "i2c_bus.h"

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* AT24C01/02每页有8个字节 
 * AT24C04/08A/16A每页有16个字节 
 */

class EEPROM {
public:
    // 单例模式
    static EEPROM& GetInstance() {
        static EEPROM instance;
        return instance;
    }
    // 禁止拷贝和赋值
    EEPROM(const EEPROM&) = delete;
    EEPROM& operator=(const EEPROM&) = delete;

    
    bool CheckOk(); // 检查设备是否正常
    bool ReadBytes(uint8_t* readBuf, uint16_t address, uint16_t size); // 读取数据
    bool WriteBytes(uint8_t* writeBuf, uint16_t address, uint16_t size); // 写入数据
    void Erase(); // 擦除EEPROM
    bool Test(); // 测试EEPROM

    // AT24CXX函数声明
    void WriteLenByte(uint16_t writeAddr, uint32_t dataToWrite, uint8_t len); // 写入指定长度的数据
    uint32_t ReadLenByte(uint16_t readAddr, uint8_t len); // 读取指定长度的数据

private:
    EEPROM() = default;  // 私有构造函数
    ~EEPROM() = default;
    

    static constexpr uint8_t EEPROM_DEVICE_ADDR = 0xA0;  // 24xx02的设备地址
    static constexpr uint8_t EEPROM_PAGE_SIZE = 8;   // 24xx02的页面大小
    static constexpr uint16_t EEPROM_SIZE = 256;      // 24xx02总容量
    static constexpr uint8_t EEPROM_WRITE_DELAY = 5;    // 写操作延时(ms)

    void Delay(uint32_t nCount);  // 简单的延时函数
};
