#include "board.h"
#include "config.h"
#include "gpio_key.h"
#include "gpio_led.h"
#include "lcd_display.h"
#include "oled_display.h"

#include "bsp.h"

// 定义全局变量，这些变量在main.o中被引用
uint8_t bsp_RunPer1ms = 0;
uint8_t bsp_RunPer10ms = 0;
uint8_t bsp_RunPer1s = 0;
uint32_t MsCount = 0;

class SchoolSTM32F103VCT6 : public Board {
  private:
    GpioKey key_;               // 声明按钮
    GpioLed led_;               // 声明LED
    LcdDisplay *lcd_display_;   // 声明液晶屏
    OledDisplay *oled_display_; // 声明OLED显示屏
  public:
    SchoolSTM32F103VCT6() {
        InitSpiBus(); // 初始化SPI总线

        seg_dz_GPIOInit();
#ifdef exti_key_demo // 如果是外部中断按键演示
        EXTI_Key_Config();
#endif
        InitLcdDisplay(); // 初始化液晶屏
        InitOledDisplay(); // 初始化OLED显示屏
        SysTick_Init();
        USART_Config();
        JQ8900setup();
        ESP_Config();
        ee_CheckOk();
        TP_Init();
        ADCx_Init();
        GENERAL_TIM_Init();
        ADVANCE_TIM_Init();
    }

    virtual Led *GetLed() override {
        static GpioLed led; // 使用GPIO控制LED
        return &led;
    }

    virtual LcdDisplay *GetLcdDisplay() override {
        return lcd_display_;
    }

    virtual OledDisplay *GetOledDisplay() override {
        return oled_display_;
    }

  private:
    void InitSpiBus() {
        bsp_InitSPIBus();
    }

    void InitLcdDisplay() {
        LCD_Init();
    }

    void InitOledDisplay() {
        oled_Init();
    }
};

DECLARE_BOARD(SchoolSTM32F103VCT6);