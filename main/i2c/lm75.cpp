/*
*********************************************************************************************************
*
*	模块名称 : LM75温度传感器类
*	文件名称 : LM75.cpp
*	版    本 : V1.0
*	说    明 : LM75温度传感器类的实现文件
*
*********************************************************************************************************
*/

#include "lm75.h"

/**
 * @brief 检查设备是否正常
 * @return 返回 true 表示正常， 返回 false 表示未探测到
 */
bool LM75::CheckOk() {
    I2CBus& i2c = I2CBus::GetInstance();
    if (i2c.CheckDevice(ADDRESS) == 0) {
        return true;
    } else {
        i2c.Stop();
        return false;
    }
}

/**
 * @brief 读取温度数据
 * @return 返回 0 表示不正常，返回非0 表示正常
 */
float LM75::ReadTemp() {
    int data;
    uint8_t tempH, tempL;
    I2CBus& i2c = I2CBus::GetInstance();

    if (!CheckOk()) {
        return 0;
    }

    i2c.Start();
    i2c.SendByte(ADDRESS);
    if (i2c.WaitAck() != 0) {
        goto LM75_fail;
    }

    i2c.SendByte(TEMP_REGISTER);
    if (i2c.WaitAck() != 0) {
        goto LM75_fail;
    }

    i2c.Start();
    i2c.SendByte(ADDRESS | I2C_RD);
    if (i2c.WaitAck() != 0) {
        goto LM75_fail;
    }

    tempH = i2c.ReadByte();
    i2c.SendAck();
    tempL = i2c.ReadByte();
    i2c.SendNAck();
    i2c.Stop();
    
    data = (tempH << 8) | tempL;
    return ((float)(data >> 5)) * 0.125;

LM75_fail: //命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备
    i2c.Stop();
    return 0;
} 