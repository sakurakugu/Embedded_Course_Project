#pragma once

#include "FreeRTOS.h"
#include "stm32f10x.h"
#include "task.h"
#include <stdio.h>
#include <new>

void Delay(__IO uint32_t nCount);
void DelayUs(uint32_t nCount);
void DelayMs(uint32_t nCount);

extern uint8_t bsp_RunPer1ms;
extern uint8_t bsp_RunPer10ms;
extern uint8_t bsp_RunPer1s;
extern uint32_t MsCount;

// FreeRTOS钩子函数声明
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
void vApplicationMallocFailedHook(void);

// void *operator new(size_t size) noexcept(false);
// void operator delete(void *pointer) noexcept;

void *operator new(size_t size);
void operator delete(void *pointer);