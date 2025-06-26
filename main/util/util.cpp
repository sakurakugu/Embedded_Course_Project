#include "util.h"

uint8_t bsp_RunPer1ms = 0;
uint8_t bsp_RunPer10ms = 0;
uint8_t bsp_RunPer1s = 0;

void Delay(__IO uint32_t nCount) // 简单的延时函数
{
    for (; nCount != 0; nCount--)
        ;
}

void DelayUs(uint32_t nCount) {
    for (; nCount != 0; nCount--)
        Delay(10);
}

uint32_t MsCount = 0;
void DelayMs(uint32_t nCount) {
    MsCount = nCount;
    while (MsCount)
        printf("The Test\n");
}

/**
 * @brief  FreeRTOS堆栈溢出钩子函数
 * @param  xTask: 发生溢出的任务句柄
 * @param  pcTaskName: 发生溢出的任务名称
 * @retval 无
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    /* 防止编译器警告 */
    (void)xTask;

    printf("任务 %s 发生堆栈溢出\r\n", pcTaskName);

    /* 无限循环，系统停止 */
    for (;;)
        ;
}

/**
 * @brief  FreeRTOS内存分配失败钩子函数
 * @param  无
 * @retval 无
 */
void vApplicationMallocFailedHook(void) {
    printf("内存分配失败\r\n");

    /* 无限循环，系统停止 */
    for (;;)
        ;
}

// void* operator new(size_t size) noexcept(false) {
//     return pvPortMalloc(size);
// }

// void operator delete(void* pointer) noexcept {
//     vPortFree(pointer);
// }

void *operator new(size_t size) {
    return malloc(size);
}

void operator delete(void *pointer) {
    free(pointer);
}
