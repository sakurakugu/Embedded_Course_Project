#include "board.h"

class SchoolSTM32F103VCT6 : public Board {
  private:
    KeyBoard *key_;              // 声明按钮指针
    // GpioLed *led_;              // 声明LED指针
    // LcdDisplay *lcd_display_;   // 声明液晶屏
    // OledDisplay *oled_display_; // 声明OLED显示屏
    // TouchScreen *touch_;        // 声明触摸屏
    // LedDisplay *led_display_;   // 声明LED显示屏
    // Gui *gui_;                  // 声明GUI对象
    // SPIBus *spi_bus_;           // 声明SPI总线
  public:
    SchoolSTM32F103VCT6() {
        LED_GPIO_Config();
        InitKey();
        bsp_InitSPIBus();
        seg_dz_GPIOInit();
    #ifdef exti_key_demo
        EXTI_Key_Config();
    #endif
        oled_Init();
        LCD_Init();
        SysTick_Init();
        USART_Config();
        JQ8900setup();
        ESP_Config();
        ee_CheckOk();
        TP_Init();
        ADCx_Init();
        GENERAL_TIM_Init();
        ADVANCE_TIM_Init();
        SetRGBColor(0x000000);
    }

//     virtual Led *GetLed() override {
//         static GpioLed led; // 使用GPIO控制LED
//         return &led;
//     }

    virtual Key *GetKey() override {
        static KeyBoard key; // 使用GPIO控制按键
        return &key;
    }

//     virtual LcdDisplay *GetLcdDisplay() override {
//         return lcd_display_;
//     }

//     virtual Gui *GetGui() override {
//         return gui_;
//     }

//     virtual TouchScreen *GetTouchScreen() override {
//         return touch_;
//     }

//     virtual OledDisplay *GetOledDisplay() override {
//         return oled_display_;
//     }

//     virtual LedDisplay *GetLedDisplay() override {
//         return led_display_;
//     }

  private:
//     void InitSpiBus() {spi_bus_ = &SPIBus::GetInstance();}
//     void InitLedDisplay() {led_display_ = new LedDisplay();}
//     void InitLcdDisplay() {lcd_display_ = new LcdDisplay();}
//     void InitTouchScreen() {
//         gui_ = new Gui();
//         touch_ = &TouchScreen::GetInstance();
//     }

//     void InitLed() {led_ = new GpioLed();}
    void InitKey() {key_ = new KeyBoard();}
//     void InitOledDisplay() {oled_display_ = new OledDisplay();}
//     void InitEEPROM() {EEPROM::GetInstance().CheckOk();}
};

DECLARE_BOARD(SchoolSTM32F103VCT6);