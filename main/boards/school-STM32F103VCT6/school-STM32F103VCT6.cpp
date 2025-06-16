#include "board.h"
#include "bsp_AdvanceTim.h"
#include "bsp_GeneralTim.h"
#include "bsp_SysTick.h"
#include "bsp_adc.h"
#include "bsp_segdz.h"
#include "bsp_spi_bus.h"
#include "config.h"
#include "gpio_key.h"
#include "gpio_led.h"
#include "gui.h"
#include "lcd_display.h"
#include "oled_display.h"
#include "touch.h"

class SchoolSTM32F103VCT6 : public Board {
  private:
    GpioKey key_;               // 声明按钮
    GpioLed led_;               // 声明LED
    LcdDisplay *lcd_display_;   // 声明液晶屏
    OledDisplay *oled_display_; // 声明OLED显示屏
    TouchScreen *touch_;        // 声明触摸屏
    Gui *gui_;                  // 声明GUI对象
  public:
    SchoolSTM32F103VCT6() {
        InitSpiBus(); // 初始化SPI总线

        seg_dz_GPIOInit();
#ifdef exti_key_demo // 如果是外部中断按键演示
        EXTI_Key_Config();
#endif
        InitLcdDisplay();  // 初始化液晶屏
        InitTouchScreen(); // 初始化触摸屏
        InitOledDisplay(); // 初始化OLED显示屏
        SysTick_Init();
        USART_Config();
        JQ8900setup();
        ESP_Config();
        ee_CheckOk();
        ADCx_Init();
        GENERAL_TIM_Init();
        ADVANCE_TIM_Init();
    }

    virtual Led *GetLed() override {
        static GpioLed led; // 使用GPIO控制LED
        return &led;
    }

    virtual Key *GetKey() override {
        static GpioKey key; // 使用GPIO控制按键
        return &key;
    }

    virtual LcdDisplay *GetLcdDisplay() override {
        return lcd_display_;
    }

    virtual Gui *GetGui() override {
        return gui_;
    }

    virtual TouchScreen *GetTouchScreen() override {
        return touch_;
    }

    virtual OledDisplay *GetOledDisplay() override {
        return oled_display_;
    }

  private:
    void InitSpiBus() {
        bsp_InitSPIBus();
    }

    void InitLcdDisplay() {
        lcd_display_ = new LcdDisplay();
    }

    void InitTouchScreen() {
        gui_ = new Gui();
        touch_ = &TouchScreen::GetInstance();
    }

    void InitOledDisplay() {
        oled_display_ = new OledDisplay();
    }
};

DECLARE_BOARD(SchoolSTM32F103VCT6);