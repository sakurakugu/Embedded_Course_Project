/*
*********************************************************************************************************
*
*	模块名称 : LM75温度传感器类
*	文件名称 : lm75.h
*	版    本 : V1.0
*	说    明 : LM75温度传感器类的头文件
*
*********************************************************************************************************
*/

#pragma once

#include "i2c_bus.h"

class LM75 {
  public:
    // 单例模式
    static LM75 &GetInstance() {
        static LM75 instance;
        return instance;
    }
    // 禁止拷贝和赋值
    LM75(const LM75 &) = delete;
    LM75 &operator=(const LM75 &) = delete;

    bool CheckOk(); // 检查设备是否正常
    float ReadTemp();  // 读取温度数据

  private:
    LM75() = default; // 私有构造函数
    ~LM75() = default;

    static constexpr uint8_t ADDRESS = 0x92;     // LM75的设备地址
    static constexpr uint8_t TEMP_REGISTER = 0;  // 温度寄存器地址
    static constexpr uint8_t CONF_REGISTER = 1;  // 配置寄存器地址
    static constexpr uint8_t THYST_REGISTER = 2; // 温度下限寄存器地址
    static constexpr uint8_t TOS_REGISTER = 3;   // 温度上限寄存器地址
};