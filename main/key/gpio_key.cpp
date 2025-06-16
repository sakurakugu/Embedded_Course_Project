#include "gpio_key.h"

GpioKey::GpioKey() {
    Init(); // 初始化按键GPIO端口
}

GpioKey::~GpioKey() {
    // 析构函数
}

void GpioKey::Init() {
    KeyGPIOConfig(); // 配置按键GPIO端口
    KeyComConfig();    // 配置按键公共端口
}

uint8_t GpioKey::ReadKey() {
    static uint8_t key_state = 0;
    uint8_t key_return = 0;
    switch (key_state) {
    case key_state_0:                // 按键初始态
        if (KeyStatusN() != ALLKEYOFF) // 读按键I/O电平
            key_state = key_state_1; // 键被按下，状态转换到键确认态
        break;
    case key_state_1: // 按键确认态
        if (KeyStatusN() != ALLKEYOFF) {
            if (KeyStatus(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON) {
                key_return = KEY1;       // 按键仍按下，按键确认输出为"1" （1）
                key_state = key_state_2; // 状态转换到键释放态
            }
            if (KeyStatus(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON) {
                key_return = KEY2;       // 按键仍按下，按键确认输出为"1" （1）
                key_state = key_state_2; // 状态转换到键释放态
            }
            if (KeyStatus(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == KEY_ON) {
                key_return = KEY3;       // 按键仍按下，按键确认输出为"1" （1）
                key_state = key_state_2; // 状态转换到键释放态
            }
            if (KeyStatus(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == KEY_ON) {
                key_return = KEY4;       // 按键仍按下，按键确认输出为"1" （1）
                key_state = key_state_2; // 状态转换到键释放态
            }
        } else
            key_state = key_state_0; // 按键已抬起，转换到按键初始态
        break;
    case key_state_2:
        if (KeyStatusN() == ALLKEYOFF) { // 按键释放
            key_state = key_state_0;
        }
        break;
    }
    return key_return;
}

void GpioKey::KeyGPIOConfig() {
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启按键端口的时钟*/
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK | KEY3_GPIO_CLK | KEY4_GPIO_CLK, ENABLE);

    // 选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
    // 设置按键的引脚为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // 使用结构体初始化按键
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

    // 选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
    // 设置按键的引脚为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // 使用结构体初始化按键
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

    // 选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
    // 设置按键的引脚为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // 使用结构体初始化按键
    GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);

    // 选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;
    // 设置按键的引脚为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // 使用结构体初始化按键
    GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);
}

void GpioKey::KeyComConfig() {
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启按键端口的时钟*/
    RCC_APB2PeriphClockCmd(KEYL_GPIO_CLK, ENABLE);

    // 按键的公共端引脚输出
    GPIO_InitStructure.GPIO_Pin = KEYL1_GPIO_PIN | KEYL2_GPIO_PIN | KEYL3_GPIO_PIN;
    // 设置按键的公共引脚为输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    // 使用结构体初始化
    GPIO_Init(KEYL_GPIO_PORT, &GPIO_InitStructure);
    // 输出0
    GPIO_ResetBits(KEYL_GPIO_PORT, KEYL1_GPIO_PIN | KEYL2_GPIO_PIN | KEYL3_GPIO_PIN);
}

uint8_t GpioKey::KeyScan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    // 检测按键状态
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON) {
        /*等待按键释放 */
        while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
            ;
        return KEY_ON; // 按键按下
    } else {
        return KEY_OFF; // 按键未按下
    }
}


uint16_t  GpioKey::KeyStatusN() {
    return KEY1_GPIO_PORT->IDR & (KEY1_GPIO_PIN | KEY2_GPIO_PIN | KEY3_GPIO_PIN | KEY4_GPIO_PIN);
}

uint8_t GpioKey::KeyStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    return GPIO_ReadInputDataBit(GPIOx,GPIO_Pin); // 读取按键输入状态
}