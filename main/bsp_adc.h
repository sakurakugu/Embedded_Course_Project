#pragma once

#include "stm32f10x.h"
#include "config.h"

void ADCx_Init(void);
uint16_t getAdc(uint8_t ADC_CHANNEL_X);
