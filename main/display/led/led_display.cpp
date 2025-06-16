#include "led_display.h"

// 构造函数
LedDisplay::LedDisplay() {
    sesg7Code = ::seg7Code;
    Disp_data = ::Disp_data;
    zdhz = ::zdhz;
    dzIndex = 0;
    segIndex = 0;

    GPIOInit(); // 初始化GPIO
}

// 数码管显示刷新函数
void LedDisplay::FlashSegment(void) {
    segIndex = 0;
    SEG595_Disable(); // OE

    hc595Send(seg7Code[Disp_data[segIndex]]);
    SEG_ST_CLK();

    segSelect(segIndex);
    SEG595_Enable();
    segIndex++;
    if (segIndex > 7) {
        segIndex = 0;
    }
}

void LedDisplay::hc595Send(uint8_t data) {
    bsp_spiWrite0(data);
}

// 选择段（数码管位置或点阵行）
void LedDisplay::segSelect(char index) {
    DISP_SEL_PORT->ODR &= ~(0x0f << 12);
    DISP_SEL_PORT->ODR |= index << 12;
}

void LedDisplay::FlashDotMatrix() {
    dzIndex = 0;
    DZ595_Disable();
    hc595Send(zdhz[5][dzIndex + 16]); // 字的右半+16
    hc595Send(zdhz[5][dzIndex]);      // 字的左半
    hc595Send(zdhz[4][dzIndex + 16]);
    hc595Send(zdhz[4][dzIndex]);
    DZ_ST_CLK();
    segSelect(dzIndex);
    DZ595_Enable();
    dzIndex++;
    if (dzIndex > 15) {
        dzIndex = 0;
    }
}

void LedDisplay::GPIOInit() {
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 打开GPIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出模式 */

    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_1 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    DISP_Enable();
    SEG595_Enable();
}

inline void LedDisplay::SEG595_Enable() {
    GPIO_ResetBits(SEG595_OE_PORT, SEG595_OE_PIN);
}
inline void LedDisplay::SEG595_Disable() {
    GPIO_SetBits(SEG595_OE_PORT, SEG595_OE_PIN);
}
inline void LedDisplay::SEG_ST_CLK() {
    GPIO_ResetBits(SEG595_ST_PORT, SEG595_ST_PIN);
    GPIO_SetBits(SEG595_ST_PORT, SEG595_ST_PIN);
}
inline void LedDisplay::DZ595_Enable() {
    GPIO_ResetBits(DZ595_OE_PORT, DZ595_OE_PIN);
}
inline void LedDisplay::DZ595_Disable() {
    GPIO_SetBits(DZ595_OE_PORT, DZ595_OE_PIN);
}
inline void LedDisplay::DZ_ST_CLK() {
    GPIO_ResetBits(DZ595_ST_PORT, DZ595_ST_PIN);
    GPIO_SetBits(DZ595_ST_PORT, DZ595_ST_PIN);
}
inline void LedDisplay::DISP_Enable() {
    GPIO_ResetBits(DISP_EN_PORT, DISP_EN_PIN);
}