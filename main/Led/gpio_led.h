#pragma once

#include "config.h"
#include "led.h"

/**
 * LED按下标置宏
 * LED点亮为高电平，设置 LED_ON=1， LED_OFF=0
 * 若LED点亮为低电平，把宏设置成LED_ON=0 ，LED_OFF=1 即可
 */
#define LED_ON	0
#define LED_OFF	1

class GpioLed : public Led {
  public:
    GpioLed();          // 构造函数
    virtual ~GpioLed(); // 析构函数

    void assert_failed(uint8_t *file, uint32_t line) {
        // 断言错误时执行的代码
        // LED1_ON;
    }

    void SetLed1(bool state);
    void SetLed2(bool state);
    void SetLed3(bool state);

    void ToggleLed1();
    void ToggleLed2();
    void ToggleLed3();

    void TurnOffAll();
    void TurnOnAll();

    void setAllLeds(bool state);
    void SetLedColor(bool r, bool g, bool b);
    void SetLedColor(uint8_t r, uint8_t g, uint8_t b);
    void SetLedColor(uint32_t color);

    void OnStateChanged() override; // 根据设备状态设置led状态

  private:
    void GPIO_ToggleBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin); // 反转GPIO引脚状态
    void Init();                                                  // 初始化LED GPIO端口
};