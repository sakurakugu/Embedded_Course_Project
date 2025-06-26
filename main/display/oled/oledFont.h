#pragma once
#include "stm32f10x.h"

typedef struct
{
    unsigned char value[4];   // UTF-8
	unsigned char data[24];   // 12x12 点阵数据
}CN_DotMatrix_12;

extern const unsigned char chinese_12_len;
extern const unsigned char  F6x8[][6];
extern const unsigned char  F8X16[];
extern const CN_DotMatrix_12 chinese_12[];

extern const unsigned char  BMP1[];




