#pragma once

#include "stm32f10x.h"
#include <stdio.h>
#include "config.h"


void USART_Config(void);
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef *pUSARTx, unsigned char *str);
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
