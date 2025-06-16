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
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
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
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机
//      T_IRQ        接          PC10         //触摸屏触摸中断信号
//      T_DO         接          PC2          //触摸屏SPI总线读信号
//      T_DIN        接          PC3          //触摸屏SPI总线写信号
//      T_CS         接          PC13         //触摸屏片选控制信号
//      T_CLK        接          PC0          //触摸屏SPI总线时钟信号
**************************************************************************************************/
/* @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 **************************************************************************************************/
#pragma once
#include "bsp.h"

// 触摸屏状态定义
#define TP_PRES_DOWN 0x80 // 触屏被按下
#define TP_CATH_PRES 0x40 // 有按键按下了

/**
 * @brief 触摸屏控制类
 * 
 * 这个类封装了所有与触摸屏相关的操作，包括初始化、校准、扫描和坐标读取等。
 * 该类使用单例模式，确保全局只有一个触摸屏控制实例。
 */
class TouchScreen {
public:
    /**
     * @brief 获取TouchScreen的单例
     * @return TouchScreen& 单例引用
     */
    static TouchScreen& GetInstance() {
        static TouchScreen instance;
        return instance;
    }

    // 禁止拷贝构造和赋值操作
    TouchScreen(const TouchScreen&) = delete;
    TouchScreen& operator=(const TouchScreen&) = delete;

    /**
     * @brief 初始化触摸屏
     * @return u8 初始化结果，0表示成功，1表示失败
     */
    u8 Init();

    /**
     * @brief 扫描触摸屏
     * @param tp 扫描模式：0-屏幕扫描，1-物理坐标
     * @return u8 触摸状态
     */
    u8 Scan(u8 tp);

    /**
     * @brief 触摸屏校准
     */
    void Adjust();

    /**
     * @brief 保存校准参数
     */
    void SaveAdjData();

    /**
     * @brief 读取校准参数
     * @return u8 读取结果，0表示成功，1表示失败
     */
    u8 GetAdjData();

    /**
     * @brief 读取触摸屏坐标
     * @param x 指向X坐标的指针
     * @param y 指向Y坐标的指针
     * @return int 读取结果，1表示有触摸，0表示无触摸
     */
    int ReadTouch(uint16_t *x, uint16_t *y);

    /**
     * @brief 画一个坐标校准点
     * @param x X坐标
     * @param y Y坐标
     * @param color 点的颜色
     */
    void DrawTouchPoint(u16 x, u16 y, u16 color);

    /**
     * @brief 画一个大点
     * @param x X坐标
     * @param y Y坐标
     * @param color 点的颜色
     */
    void DrawBigPoint(u16 x, u16 y, u16 color);

    /**
     * @brief 显示校准信息
     * @param x0 第一个点的X坐标
     * @param y0 第一个点的Y坐标
     * @param x1 第二个点的X坐标
     * @param y1 第二个点的Y坐标
     * @param x2 第三个点的X坐标
     * @param y2 第三个点的Y坐标
     * @param x3 第四个点的X坐标
     * @param y3 第四个点的Y坐标
     * @param fac 校准系数
     */
    void ShowAdjInfo(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac);

    /**
     * @brief 运行触摸屏测试
     */
    void Test();

    /**
     * @brief 获取原始X坐标
     * @return u16 原始X坐标
     */
    u16 GetX0() const { return x0; }

    /**
     * @brief 获取原始Y坐标
     * @return u16 原始Y坐标
     */
    u16 GetY0() const { return y0; }

    /**
     * @brief 获取当前X坐标
     * @return u16 当前X坐标
     */
    u16 GetX() const { return x; }

    /**
     * @brief 获取当前Y坐标
     * @return u16 当前Y坐标
     */
    u16 GetY() const { return y; }

    /**
     * @brief 获取触摸状态
     * @return u8 触摸状态
     */
    u8 GetStatus() const { return sta; }

private:
    // 私有构造函数，实现单例模式
    TouchScreen();
    ~TouchScreen();

    /**
     * @brief 向触摸屏控制芯片写入一个字节数据
     * @param num 要写入的数据
     */
    void WriteByte(u8 num);

    /**
     * @brief 读取AD转换值
     * @param CMD 命令
     * @return u16 AD转换值
     */
    u16 ReadAD(u8 CMD);

    /**
     * @brief 带滤波的坐标读取
     * @param xy 坐标类型：0-X坐标，1-Y坐标
     * @return u16 读取的坐标值
     */
    u16 ReadXOY(u8 xy);

    /**
     * @brief 双方向读取坐标
     * @param x 指向X坐标的指针
     * @param y 指向Y坐标的指针
     * @return u8 读取结果，0表示有效，1表示无效
     */
    u8 ReadXY(u16 *x, u16 *y);

    /**
     * @brief 带加强滤波的双方向坐标读取
     * @param x 指向X坐标的指针
     * @param y 指向Y坐标的指针
     * @return u8 读取结果，0表示有效，1表示无效
     */
    u8 ReadXY2(u16 *x, u16 *y);

    /**
     * @brief 设置触摸片选信号
     * @param bit 信号值
     */
    void SetTCS(uint8_t bit);

    /**
     * @brief 读取触摸中断信号
     * @return uint8_t 中断信号值
     */
    uint8_t ReadPEN();

    /**
     * @brief 设置SPI速度
     * @param SPI_BaudRatePrescaler SPI波特率预分频值
     */
    void SetSPISpeed(uint16_t SPI_BaudRatePrescaler);

    /**
     * @brief 写入EEPROM
     * @param WriteAddr 写入地址
     * @param DataToWrite 要写入的数据
     * @param Len 长度
     */
    void WriteEEPROM(u16 WriteAddr, u32 DataToWrite, u8 Len);

    /**
     * @brief 读取EEPROM
     * @param ReadAddr 读取地址
     * @param Len 长度
     * @return u32 读取的数据
     */
    u32 ReadEEPROM(u16 ReadAddr, u8 Len);

private:
    u16 x0;       // 原始坐标(第一次按下时的坐标)
    u16 y0;
    u16 x;        // 当前坐标(此次扫描时,触屏的坐标)
    u16 y;
    u8 sta;       // 笔的状态
                  // b7:按下1/松开0;
                  // b6:0,没有按键按下;1,有按键按下.

    // 触摸屏校准参数
    float xfac;
    float yfac;
    short xoff;
    short yoff;
    u8 touchtype; // 触摸类型
};

// 为了兼容旧代码，提供一些全局函数作为TouchScreen类方法的包装
#ifdef __cplusplus
extern "C" {
#endif
    /**
     * @brief 初始化触摸屏，C接口包装
     * @return u8 初始化结果
     */
    u8 TP_Init(void);

    /**
     * @brief 扫描触摸屏，C接口包装
     * @param tp 扫描模式
     * @return u8 触摸状态
     */
    u8 TP_Scan(u8 tp);

    /**
     * @brief 触摸屏校准，C接口包装
     */
    void TP_Adjust(void);

    /**
     * @brief 保存校准参数，C接口包装
     */
    void TP_Save_Adjdata(void);

    /**
     * @brief 读取校准参数，C接口包装
     * @return u8 读取结果
     */
    u8 TP_Get_Adjdata(void);

    /**
     * @brief 读取触摸屏坐标，C接口包装
     * @param x 指向X坐标的指针
     * @param y 指向Y坐标的指针
     * @return int 读取结果
     */
    int touch_read(uint16_t *x, uint16_t *y);

    /**
     * @brief 触摸屏测试，C接口包装
     */
    void TP_test(void);

    /**
     * @brief 显示校准信息，C接口包装
     */
    void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac);

    // 低级函数，为兼容旧代码保留
    void TCS(uint8_t bit);
    uint8_t PEN(void);
    void TPSPI_SpeedToggle(uint16_t SPI_BaudRatePrescaler);
    void TP_Write_Byte(u8 num);
    u16 TP_Read_AD(u8 CMD);
    u16 TP_Read_XOY(u8 xy);
    u8 TP_Read_XY(u16 *x, u16 *y);
    u8 TP_Read_XY2(u16 *x, u16 *y);
    void TP_Drow_Touch_Point(u16 x, u16 y, u16 color);
    void TP_Draw_Big_Point(u16 x, u16 y, u16 color);
    void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len);
    u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len);
}

#ifdef __cplusplus
}
#endif
