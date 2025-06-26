#pragma once

#include "key.h"
#include "config.h"
#include "stm32f10x.h"

/** 按键按下标置宏
 *  按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
 *  若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
 */
#define KEY_ON	0
#define KEY_OFF	1

#define ROW_NUM 3
#define COL_NUM 4

class KeyBoard : public Key{
private:
    static uint8_t key_state;
    static uint8_t timekey;
    void Key_GPIO_Config();
    void Key_Com_GPIO_Config();
    
public:
    KeyBoard();
    ~KeyBoard();
    
    uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
    
    uint8_t ReadKey() override;
    uint8_t ReadKeyN() override;
};


