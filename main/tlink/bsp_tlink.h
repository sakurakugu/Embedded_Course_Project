#pragma once

/*
    stm32f10x.h :STM32F10x系列CPU的标准外设固件库。这个固件包含了很多型号的CPU，
        使用这个库必须在工程中添加预编译的宏:
            USE_STDPERIPH_DRIVER  表示启用标准外设库
            STM32F10X_HD          表示CPU是10x大容量版本(HD = High Density)

    stdlib.h : standard library标准库头文件,包含了C、C++语言的最常用的类型定义和系统函数，比如 malloc()、free()

    stdio.h : standard input & output 标准输入输出函数库。常用函数有 printf()、getc()、putc()

    string.h : 关于字符数组的函数定义的头文件库，常用函数有strlen、strcmp、strcpy

    " " 和 < > 的区别:
    " " 一般表示源代码自带的h文件。编译时，编译器优先在工程配置指定的搜索路径列表中寻找头文件
    < > 一般表示编译系统自带的h文件。编译时，编译器优先在IDE自带的库文件夹寻找头文件。
*/
#include "bsp_ESPxx.h"
#include "bsp_usart.h"
#include "config.h"
#include "stm32f10x.h"
#include <string.h>

#define  TLINK_OK_LED_ON()					GPIO_ResetBits(Tlink_OK_PORT,Tlink_OK_GPIO_PIN);
#define  TLINK_OK_LED_OFF()					GPIO_SetBits(Tlink_OK_PORT,Tlink_OK_GPIO_PIN);

extern unsigned char time1sflag;
extern unsigned char timensflag;
extern unsigned char linkokflag;

extern unsigned char Txto8266Buf[];
extern unsigned char e8266tomcu_str[];
extern unsigned int e8266tomcu_num;

unsigned char E8266inittotlink(void);
