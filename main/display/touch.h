//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
// QDtech-TFT液晶驱动 for STM32 IO模拟
// xiao冯@ShenZhen QDtech co.,LTD
// 公司网站:www.qdtft.com
// 淘宝网站：http://qdtech.taobao.com
// wiki技术网站：http://www.lcdwiki.com
// 我司提供技术支持，任何技术问题欢迎随时交流学习
// 固话(传真) :+86 0755-23594567
// 手机:15989313508（冯工）
// 邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
// 技术支持QQ:3002773612  3002778157
// 技术交流QQ群:324828016
// 创建日期:2018/08/09
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 深圳市全动电子技术有限公司 2018-2028
// All rights reserved
/*****************************************************************************************************/
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
// 本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机
//    SDI(MOSI)      接          PB15         //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PB14         //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机
//       LED         接          PB9          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB13         //液晶屏SPI总线时钟信号
//      DC/RS        接          PB10         //液晶屏数据/命令控制信号
//       RST         接          PB12         //液晶屏复位控制信号
//       CS          接          PB11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
// 如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机
//      T_IRQ        接          PC10         //触摸屏触摸中断信号
//      T_DO         接          PC2          //触摸屏SPI总线读信号
//      T_DIN        接          PC3          //触摸屏SPI总线写信号
//      T_CS         接          PC13         //触摸屏片选控制信号
//      T_CLK        接          PC0          //触摸屏SPI总线时钟信号
/***************************************************************************************************/
/* @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *************************************************************************************************
 */
#pragma once
#include "bsp.h"

#define SPI_Mode_Select ((uint16_t)0xF7FF)
#define SPI_HARD SPI1

// 触摸屏状态定义
#define TP_PRES_DOWN 0x80 // 触屏被按下
#define TP_CATH_PRES 0x40 // 有按键按下了

// 保存在EEPROM里面的地址区间基址,占用13个字节
// (RANGE : SAVE_ADDR_BASE ~SAVE_ADDR_BASE + 12)
#define SAVE_ADDR_BASE 255 - 13

#define READ_TIMES 5 // 读取次数
#define LOST_VAL 1   // 丢弃值

#define ERR_RANGE 50 // 误差范围

    /**
     * @brief 触摸屏控制类
     *
     * 这个类封装了所有与触摸屏相关的操作，包括初始化、校准、扫描和坐标读取等。
     * 该类使用单例模式，确保全局只有一个触摸屏控制实例。
     */
    class TouchScreen {
  public:
    static TouchScreen &GetInstance() {
        static TouchScreen instance;
        return instance;
    }

    // 禁止拷贝构造和赋值操作
    TouchScreen(const TouchScreen &) = delete;
    TouchScreen &operator=(const TouchScreen &) = delete;
    u8 Init();
    u8 Scan(u8 tp);
    void Adjust();
    void SaveAdjData();
    u8 GetAdjData();
    int ReadTouch(uint16_t *x, uint16_t *y);
    void DrawTouchPoint(u16 x, u16 y, u16 color);
    void DrawBigPoint(u16 x, u16 y, u16 color);
    void ShowAdjInfo(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac);
    void Test();
    u16 GetX0() const {
        return x0;
    }
    u16 GetY0() const {
        return y0;
    }
    u16 GetX() const {
        return x;
    }
    u16 GetY() const {
        return y;
    }
    u8 GetStatus() const {
        return sta;
    }

  private:
    // 私有构造函数，实现单例模式
    TouchScreen();
    ~TouchScreen();
    void WriteByte(u8 num);
    u16 ReadAD(u8 CMD);
    u16 ReadXOY(u8 xy);
    u8 ReadXY(u16 *x, u16 *y);
    u8 ReadXY2(u16 *x, u16 *y);
    void SetTCS(uint8_t bit);
    uint8_t ReadPEN();
    void SetSPISpeed(uint16_t SPI_BaudRatePrescaler);
    void WriteEEPROM(u16 WriteAddr, u32 DataToWrite, u8 Len);
    u32 ReadEEPROM(u16 ReadAddr, u8 Len);

  private:
    u16 x0; // 原始坐标(第一次按下时的坐标)
    u16 y0;
    u16 x; // 当前坐标(此次扫描时,触屏的坐标)
    u16 y;
    u8 sta; // 笔的状态
            // b7:按下1/松开0;
            // b6:0,没有按键按下;1,有按键按下.

    // 触摸屏校准参数
    float xfac;
    float yfac;
    short xoff;
    short yoff;
    u8 touchtype; // 触摸类型

    u8 CMD_RDX;
    u8 CMD_RDY;
};
