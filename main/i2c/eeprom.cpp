/*
*********************************************************************************************************
*
*	模块名称 : EEPROM类
*	文件名称 : EEPROM.cpp
*	版    本 : V1.0
*	说    明 : EEPROM类的实现文件
*
*********************************************************************************************************
*/

#include "eeprom.h"

/**
 * @brief 检查设备是否正常
 * @return 返回 true 表示正常， 返回 false 表示不正常
 */
bool EEPROM::CheckOk() {
    I2CBus& i2c = I2CBus::GetInstance();
    if (i2c.CheckDevice(EEPROM_DEVICE_ADDR) == 0) {
        return true;
    } else {
        i2c.Stop();
        return false;
    }
}

/**
 * @brief 读取数据
 * @param readBuf 读取数据的缓冲区
 * @param address 起始地址
 * @param size 读取数据的长度
 * @return 返回 true 表示正常， 返回 false 表示不正常
 */
bool EEPROM::ReadBytes(uint8_t* readBuf, uint16_t address, uint16_t size) {
    uint16_t i;
    // 采用串行EEPROM随即读取指令序列，连续读取若干字节
    // 第1步：发起I2C总线启动信号
    I2CBus& i2c = I2CBus::GetInstance();

    // 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读
    i2c.Start();
    i2c.SendByte(EEPROM_DEVICE_ADDR | I2C_WR);

    // 第3步：等待ACK
    if (i2c.WaitAck() != 0) {
        goto cmd_fail;
    }

    // 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址
    i2c.SendByte((uint8_t)address);

    // 第5步：等待ACK
    if (i2c.WaitAck() != 0) {
        goto cmd_fail;
    }

    // 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据
    i2c.Start();

    // 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读
    i2c.SendByte(EEPROM_DEVICE_ADDR | I2C_RD);

    // 第8步：等待ACK
    if (i2c.WaitAck() != 0) {
        goto cmd_fail;
    }

    // 第9步：读取数据
    for (i = 0; i < size; i++) {
        readBuf[i] = i2c.ReadByte();
        if (i != size - 1) {
            i2c.SendAck();
        } else {
            i2c.SendNAck();
        }
    }

    // 第10步：发送停止信号
    i2c.Stop();
    return true;

cmd_fail:
    i2c.Stop();
    return false;
}

/**
 * @brief 写入数据
 * @param writeBuf 写入数据的缓冲区
 * @param address 起始地址
 * @param size 写入数据的长度
 * @return 返回 true 表示正常， 返回 false 表示不正常
 */
bool EEPROM::WriteBytes(uint8_t* writeBuf, uint16_t address, uint16_t size) {
    uint16_t i, m;
    uint16_t usAddr;
    I2CBus& i2c = I2CBus::GetInstance();

    /*
        写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
        对于24xx02，page size = 8
        简单的处理方法为：按字节写操作模式，每写1个字节，都发送地址
        为了提高连续写的效率: 本函数采用page wirte操作。
    */

    usAddr = address;
    for (i = 0; i < size; i++) {
        // 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址
        if ((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0) {
            // 第1步：发停止信号，启动内部写操作
            i2c.Stop();

            // 第2步：通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
            // CLK频率为200KHz时，查询次数为30次左右
            for (m = 0; m < 1000; m++) {
                // 第(1)步：发起I2C总线启动信号
                i2c.Start();

                // 第(2)步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读
                i2c.SendByte(EEPROM_DEVICE_ADDR | I2C_WR);

                // 第(3)步：发送一个时钟，判断器件是否正确应答
                if (i2c.WaitAck() == 0) {
                    break;
                }
            }
            if (m == 1000) {
                goto cmd_fail;
            }

            // 第3步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址
            i2c.SendByte((uint8_t)usAddr);

            // 第4步：等待ACK
            if (i2c.WaitAck() != 0) {
                goto cmd_fail;
            }
        }

        // 第5步：开始写入数据
        i2c.SendByte(writeBuf[i]);

        // 第6步：发送ACK
        if (i2c.WaitAck() != 0) {
            goto cmd_fail;
        }

        // 第7步：地址递增
        usAddr++;
    }

    i2c.Stop();
    return true;

cmd_fail:
    i2c.Stop();
    return false;
}

/**
 * @brief 擦除EEPROM
 */
void EEPROM::Erase() {
    uint16_t i;
    uint8_t buf[EEPROM_SIZE];

    // 填充缓冲区
    for (i = 0; i < EEPROM_SIZE; i++) {
        buf[i] = 0xFF;
    }

    // 写EEPROM, 起始地址 = 0，数据长度为 256
    if (WriteBytes(buf, 0, EEPROM_SIZE) == 0) {
        return;
    }
}

/**
 * @brief 简单的延时函数
 * @param nCount 延时计数
 */
void EEPROM::Delay(uint32_t nCount) {
    for (; nCount != 0; nCount--);
}

/**
 * @brief 测试EEPROM
 * @return 返回 true 表示正常， 返回 false 表示不正常
 */
bool EEPROM::Test() {
    uint16_t i;
    uint8_t write_buf[EEPROM_SIZE];
    uint8_t read_buf[EEPROM_SIZE];

    // 检查设备是否正常
    if (CheckOk() == 0) {
        return false;
    }

    // 填充缓冲区
    for (i = 0; i < EEPROM_SIZE; i++) {
        write_buf[i] = i;
    }

    // 写EEPROM, 起始地址 = 0，数据长度为 256
    if (WriteBytes(write_buf, 0, EEPROM_SIZE) == 0) {
        return false;
    }

    // 延时
    Delay(0x0FFFFF);

    // 读EEPROM, 起始地址 = 0，数据长度为 256
    if (ReadBytes(read_buf, 0, EEPROM_SIZE) == 0) {
        return false;
    }

    // 比较读取的数据和写入的数据
    for (i = 0; i < EEPROM_SIZE; i++) {
        if (read_buf[i] != write_buf[i]) {
            return false;
        }
    }

    // 测试成功
    return true;
}

// AT24CXX函数实现

/**
 * @brief 写入指定长度的数据
 * @param writeAddr 写入地址
 * @param dataToWrite 写入数据
 * @param len 写入长度
 */
void EEPROM::WriteLenByte(uint16_t writeAddr, uint32_t dataToWrite, uint8_t len) {
    uint8_t t;
    for(t=0; t<len; t++) {
        WriteBytes((uint8_t *)&dataToWrite, writeAddr+t, 1);
    }
}

/**
 * @brief 读取指定长度的数据
 * @param readAddr 读取地址
 * @param len 读取长度
 * @return 返回读取的数据
 */
uint32_t EEPROM::ReadLenByte(uint16_t readAddr, uint8_t len) {
    uint8_t t;
    uint32_t temp=0;
    for(t=0; t<len; t++) {
        temp<<=8;
        ReadBytes((uint8_t *)&temp, readAddr+t, 1);
    }
    return temp;
} 