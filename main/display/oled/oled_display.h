#pragma once

#include "stm32f10x.h"
#include "oled_font.h"
#include "display.h"
#include "bsp_SysTick.h"
#include "spi_bus.h"

// 前向声明
class Board;

// OLED模式设置常量
#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据
#define OLED_MODE 0 // 有DC线

// OLED模式设置
// 0:4线串行模式
// 1:并行8080模式

// OLED尺寸常量
#define OLED_SIZE        16
#define OLED_XLevelL     0x02
#define OLED_XLevelH     0x10
#define OLED_Max_Column  128
#define OLED_Max_Row     64
#define OLED_Brightness  0xFF 
#define OLED_X_WIDTH     128
#define OLED_Y_WIDTH     64     
#define OLED_W      OLED_X_WIDTH
#define OLED_H      OLED_Y_WIDTH        
#define OLED_GRAM_H      OLED_H/8

#define OLED_DC_SET() GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET)
#define OLED_DC_CLR() GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET)

#define OLED_CS_SET() GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET)
#define OLED_CS_CLR() GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET)

#define OLED_RST_SET() GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_SET)
#define OLED_RST_CLR() GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_RESET)

class OledDisplay : public Display {
  public:
    OledDisplay();
    ~OledDisplay();

    void DisplayOn();
    void DisplayOff();
    void GpioInit();
    void Init();
    void DrawPoint(uint8_t x, uint8_t y, uint8_t t);
    void Clear();
    void Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
    void ShowChar(uint8_t x, uint8_t y, uint8_t ch);
    void WriteByte(uint8_t dat, uint8_t cmd);
    void ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
    void ShowChinese(uint8_t x, uint8_t y, uint8_t no);
    void ShowString(uint8_t x, uint8_t y, uint8_t *p);
    void SetPos(uint8_t x, uint8_t y);
    void ShowCHStr(uint8_t X, uint8_t Y, uint8_t *pstr);
    void DrawBMP(uint8_t X, uint8_t Y, uint8_t Pix_x, uint8_t Pix_y, const uint8_t *BMP);

  private:
    SPIBus &spi_bus_; // 声明SPI总线
};

class NoOledDisplay : public OledDisplay {
  public:
    NoOledDisplay() : OledDisplay() {
    }
    ~NoOledDisplay() {
    }
};

uint32_t Pow(uint8_t m, uint8_t n);
