#pragma once

#include "spi_bus.h"
#include "config.h"
#include "display.h"
#include "led_font.h"

// 声明外部变量

class LedDisplay : public Display {
  private:
    const uint8_t *sesg7Code;        // 段码数据
    uint8_t *Disp_data;        // 显示数据
    const uint8_t (*zdhz)[32]; // 汉字点阵数据
    int dzIndex;               // 点阵索引
    int segIndex;              // 段码索引
    SPIBus &spi_bus_;          // 声明SPI总线

    inline void hc595Send(uint8_t data); // 向HC595七位数码管发送数据
    inline void SEG595_Enable();         // 开启数码管
    inline void SEG595_Disable();        // 禁用数码管
    inline void SEG_ST_CLK();            // 数码管时钟脉冲
    inline void DZ595_Enable();          // 开启点阵
    inline void DZ595_Disable();         // 禁用点阵
    inline void DZ_ST_CLK();             // 点阵时钟脉冲
    inline void DISP_Enable();           // 开启显示

  public:
    LedDisplay();                     // 构造函数
    ~LedDisplay() override = default; // 析构函数
    void GPIOInit();                  // 初始化GPIO
    void segSelect(char index);       // 选择段（数码管位置或点阵行）
    void FlashDotMatrix();            // 点阵显示刷新函数
    void FlashSegment();              // 数码管显示刷新函数
};

class NoLedDisplay : public LedDisplay {
  public:
    NoLedDisplay() : LedDisplay() {
    }
    ~NoLedDisplay() {
    }
};