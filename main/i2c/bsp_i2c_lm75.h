#pragma once


#include "stm32f10x.h"
#include "bsp_i2c_gpio.h"
#include <stdio.h>

#define LM75_ADDRESS   0x92

#define LM75_TEMP_REGISTER 0
#define LM75_CONF_REGISTER 1
#define LM75_THYST_REGISTER 2
#define LM75_TOS_REGISTER 3

uint8_t LM75_CheckOk(void);
float LM75_readTemp(void);



