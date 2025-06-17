#pragma once

#include "stm32f10x.h"
#include "display.h"
#include "spi_bus.h"
#include "bsp_SysTick.h"

// 定义颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0

#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
#define LIGHTGREEN 0X841F
#define LIGHTGRAY 0XEF5B
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12

// LCD重要参数集
struct lcd_dev_ {
    u16 width;   // LCD宽度
    u16 height;  // LCD高度
    u16 id;      // LCD ID
    u8 dir;      // 横屏还是竖屏控制：0，竖屏；1，横屏。
    u16 wramcmd; // 开始写gram指令
    u16 setxcmd; // 设置x坐标指令
    u16 setycmd; // 设置y坐标指令
};

// 定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转
#define USE_HORIZONTAL 0
// 定义LCD的尺寸
#define LCD_W 320
#define LCD_H 480

#define LCD_DC_SET() GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET)
#define LCD_DC_CLR() GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET)

#define LCD_CS_SET() GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET)
#define LCD_CS_CLR() GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_RESET)

#define LCD_RST_SET() GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_SET)
#define LCD_RST_CLR() GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_RESET)

#define LCD_LED_ON() GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET)
#define LCD_LED_OFF() GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET)

class LcdDisplay : public Display {
  public:
    LcdDisplay();
    ~LcdDisplay() override;

    // 从bsp_lcd.h移植的方法
    void Init();
    void DisplayOn();
    void DisplayOff();
    u16 ReadPoint(u16 x, u16 y);
    void DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
    void DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
    u16 RD_DATA();
    void WR_DATA(u8 data);
    void WR_REG(u8 data);
    u16 ReadReg(u8 LCD_Reg);
    void WriteRAM_Prepare();
    u16 ReadRAM();
    u16 BGR2RGB(u16 c);
    void SetParam();
    void SetDirection(u8 direction);
    void WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
    void WriteRAM(u16 RGB_Code);
    void DrawPoint(u16 x, u16 y);
    void Clear(u16 Color);
    void SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd);
    void SetCursor(u16 Xpos, u16 Ypos);
    void WriteData_16Bit(u16 Data);
    void GPIOInit();

  // protected:
    lcd_dev_ lcddev; // 管理LCD重要参数，默认为竖屏
    u16 POINT_COLOR; // 默认红色
    u16 BACK_COLOR;  // 背景颜色，默认为白色

  private:
    SPIBus &spi_bus_; // 声明SPI总线
};

class NoLcdDisplay : public LcdDisplay {
  public:
    NoLcdDisplay() : LcdDisplay() {
    }
    ~NoLcdDisplay() {
    }
};