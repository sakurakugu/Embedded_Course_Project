#pragma once

#include "config.h"
#include "key.h"

/**
 * 按键按下标置宏
 * 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
 * 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
 */
#define KEY_ON 0
#define KEY_OFF 1

enum KeyState {
    key_state_0, // 按键初始态
    key_state_1, // 按键确认态
    key_state_2, // 按键释放态
    key_state_3  // 按键长按态
};

constexpr uint16_t ALLKEYOFF = KEY1_GPIO_PIN | KEY2_GPIO_PIN | KEY3_GPIO_PIN | KEY4_GPIO_PIN;

enum KeyInput {
    KEY1 = 1, // 按键1按下
    KEY2 = 2,
    KEY3 = 3,
    KEY4 = 4
};

class GpioKey : public Key {
  public:
    GpioKey();
    virtual ~GpioKey();

    void Init();       // 初始化按键GPIO端口
    uint8_t ReadKey(); // 读取按键状态

    uint8_t KeyScan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin); // 检测按键状态
  private:
    void KeyGPIOConfig();                                   // 配置按键GPIO端口
    void KeyComConfig();                                    // 配置按键公共端口
    inline uint16_t KeyStatusN();                               // 检测按键状态
    inline uint8_t KeyStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin); // 读取按键输入状态
};