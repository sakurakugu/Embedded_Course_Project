#include "gpio_led.h"

GpioLed::GpioLed() {
    // 构造函数
}

GpioLed::~GpioLed() {
    // 析构函数
}

void GpioLed::Init() {
    // 初始化LED GPIO端口
    GPIO_InitTypeDef GPIO_InitStructure;

    // 开启LED GPIO端口时钟
    RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);

    // 配置LED1引脚
    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

    // 配置LED2引脚
    GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

    // 配置LED3引脚
    GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

    // 初始化LED状态为关闭
    TurnOffAll();
}

void GpioLed::SetLed1(bool state) {
    if (state) {
        GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
    } else {
        GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
    }
}

void GpioLed::SetLed2(bool state) {
    if (state) {
        GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
    } else {
        GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
    }
}

void GpioLed::SetLed3(bool state) {
    if (state) {
        GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
    } else {
        GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
    }
}

void GpioLed::GPIO_ToggleBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    GPIOx->ODR ^= GPIO_Pin; // 输出反转状态
}

void GpioLed::ToggleLed1() {
    GPIO_ToggleBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}
void GpioLed::ToggleLed2() {
    GPIO_ToggleBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
}
void GpioLed::ToggleLed3() {
    GPIO_ToggleBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

void GpioLed::TurnOffAll() {
    SetLed1(false);
    SetLed2(false);
    SetLed3(false);
}
void GpioLed::TurnOnAll() {
    SetLed1(true);
    SetLed2(true);
    SetLed3(true);
}

void GpioLed::setAllLeds(bool state) {
    SetLed1(state);
    SetLed2(state);
    SetLed3(state);
}
void GpioLed::SetLedColor(bool r, bool g, bool b) {
    SetLed1(r);
    SetLed2(g);
    SetLed3(b);
}
void GpioLed::SetLedColor(uint8_t r, uint8_t g, uint8_t b) {
    SetLed1(r > 0);
    SetLed2(g > 0);
    SetLed3(b > 0);
}
void GpioLed::SetLedColor(uint32_t color) {
    SetLed1((color & 0xFF0000) != 0);
    SetLed2((color & 0x00FF00) != 0);
    SetLed3((color & 0x0000FF) != 0);
}