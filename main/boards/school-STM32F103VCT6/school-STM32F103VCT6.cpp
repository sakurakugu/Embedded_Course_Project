#include "board.h"
#include "bsp_AdvanceTim.h"
#include "bsp_GeneralTim.h"
#include "bsp_SysTick.h"
#include "bsp_adc.h"
#include "bsp_spi_bus.h"
#include "config.h"
#include "gpio_key.h"
#include "gpio_led.h"
#include "gui.h"
#include "lcd_display.h"
#include "led_display.h"
#include "oled_display.h"
#include "touch.h"
#include "bsp_usart.h"
#include "bsp_JQ8900.h"
#include "bsp_ESPxx.h"
#include "bsp_tlink.h"
#include "bsp_i2c_gpio.h"
#include "bsp_i2c_ee.h"
#include "bsp_i2c_lm75.h"

class SchoolSTM32F103VCT6 : public Board {
  private:
    GpioKey *key_;              // 声明按钮指针
    GpioLed *led_;              // 声明LED指针
    LcdDisplay *lcd_display_;   // 声明液晶屏
    OledDisplay *oled_display_; // 声明OLED显示屏
    TouchScreen *touch_;        // 声明触摸屏
    LedDisplay *led_display_;   // 声明LED显示屏
    Gui *gui_;                  // 声明GUI对象
  public:
    SchoolSTM32F103VCT6() {
        InitLed();
        InitKey();
        InitSpiBus(); // 初始化SPI总线
        InitLedDisplay();
#ifdef exti_key_demo // 如果是外部中断按键演示
        EXTI_Key_Config();
#endif
        InitOledDisplay(); // 初始化OLED显示屏
        InitLcdDisplay();  // 初始化液晶屏
        SysTick_Init();
        USART_Config();
        printf("USART_Config\r\n");
        JQ8900setup();
        ESP_Config();
        ee_CheckOk();
        InitTouchScreen(); // 初始化触摸屏
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

    virtual LedDisplay *GetLedDisplay() override {
        return led_display_;
    }

  private:
    void InitSpiBus() {
        bsp_InitSPIBus();
    }

    void InitLedDisplay() {
        led_display_ = new LedDisplay();
    }

    void InitLcdDisplay() {
        lcd_display_ = new LcdDisplay();
    }

    void InitTouchScreen() {
        gui_ = new Gui();
        touch_ = &TouchScreen::GetInstance();
    }

    void InitLed() {
        led_ = new GpioLed();
    }

    void InitKey() {
        key_ = new GpioKey();
    }

    void InitOledDisplay() {
        oled_display_ = new OledDisplay();
    }
};

DECLARE_BOARD(SchoolSTM32F103VCT6);