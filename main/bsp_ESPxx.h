#pragma once

#include "bsp_tlink.h"
#include "stm32f10x.h"
#include "config.h"

void ESP_Config(void);
void ESP_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void ESP_SendString(USART_TypeDef *pUSARTx, unsigned char *str);
void ESP_SendCommand(unsigned char *str, unsigned char num);
