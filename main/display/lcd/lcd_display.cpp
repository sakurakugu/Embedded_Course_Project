#include "lcd_display.h"

LcdDisplay::LcdDisplay() : spi_bus_(SPIBus::GetInstance()) {
    POINT_COLOR = RED;  // 默认红色
    BACK_COLOR = WHITE; // 默认背景白色
    Init();             // 初始化LCD显示
}

LcdDisplay::~LcdDisplay() {
    // 释放资源或进行必要的清理
}

void LcdDisplay::GPIOInit() {
    // 初始化LCD相关GPIO
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 打开GPIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_12;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void LcdDisplay::Init() {
    GPIOInit(); // LCD
    LCD_LED_ON();
    //************* ILI9488**********//
    WR_REG(0XF7);
    WR_DATA(0xA9);
    WR_DATA(0x51);
    WR_DATA(0x2C);
    WR_DATA(0x82);
    WR_REG(0xC0);
    WR_DATA(0x11);
    WR_DATA(0x09);
    WR_REG(0xC1);
    WR_DATA(0x41);
    WR_REG(0XC5);
    WR_DATA(0x00);
    WR_DATA(0x0A);
    WR_DATA(0x80);
    WR_REG(0xB1);
    WR_DATA(0xB0);
    WR_DATA(0x11);
    WR_REG(0xB4);
    WR_DATA(0x02);
    WR_REG(0xB6);
    WR_DATA(0x02);
    WR_DATA(0x42);
    WR_REG(0xB7);
    WR_DATA(0xc6);
    WR_REG(0xBE);
    WR_DATA(0x00);
    WR_DATA(0x04);
    WR_REG(0xE9);
    WR_DATA(0x00);
    WR_REG(0x36);
    WR_DATA((1 << 3) | (0 << 7) | (1 << 6) | (1 << 5));
    WR_REG(0x3A);
    WR_DATA(0x66);
    WR_REG(0xE0);
    WR_DATA(0x00);
    WR_DATA(0x07);
    WR_DATA(0x10);
    WR_DATA(0x09);
    WR_DATA(0x17);
    WR_DATA(0x0B);
    WR_DATA(0x41);
    WR_DATA(0x89);
    WR_DATA(0x4B);
    WR_DATA(0x0A);
    WR_DATA(0x0C);
    WR_DATA(0x0E);
    WR_DATA(0x18);
    WR_DATA(0x1B);
    WR_DATA(0x0F);
    WR_REG(0XE1);
    WR_DATA(0x00);
    WR_DATA(0x17);
    WR_DATA(0x1A);
    WR_DATA(0x04);
    WR_DATA(0x0E);
    WR_DATA(0x06);
    WR_DATA(0x2F);
    WR_DATA(0x45);
    WR_DATA(0x43);
    WR_DATA(0x02);
    WR_DATA(0x0A);
    WR_DATA(0x09);
    WR_DATA(0x32);
    WR_DATA(0x36);
    WR_DATA(0x0F);
    WR_REG(0x11);
    // bsp_DelayMS(120);
    SysTick_Delay_Ms(120);
    WR_REG(0x29);

    SetDirection(2); ////设置LCD显示方向
    SysTick_Delay_Ms(1200);
    Clear(GBLUE); ////清全屏
}

void LcdDisplay::DisplayOn() {
}

void LcdDisplay::DisplayOff() {
}

u16 LcdDisplay::ReadPoint(u16 x, u16 y) {
    return 0;
}

void LcdDisplay::DrawLine(u16 x1, u16 y1, u16 x2, u16 y2) {
}

void LcdDisplay::DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2) {
}

u16 LcdDisplay::RD_DATA() {
    return 0;
}

/**
 * @brief 向LCD写入8位数据
 * @param data: 要写入的数据
 */
void LcdDisplay::WR_DATA(u8 data) {
    LCD_DC_SET();
    // DISABLE_INT();
    LCD_CS_CLR();
    spi_bus_.Write0(data);
    LCD_CS_SET();
    // ENABLE_INT();
}

/**
 * @brief 向LCD寄存器写入8位命令
 * @param data: 要写入的命令
 */
void LcdDisplay::WR_REG(u8 data) {
    LCD_DC_CLR();
    // DISABLE_INT();
    LCD_CS_CLR();
    spi_bus_.Write0(data);
    LCD_CS_SET();
    // ENABLE_INT();
}

u16 LcdDisplay::ReadReg(u8 LCD_Reg) {
    return 0;
}

/**
 * @brief 准备写入显存GRAM
 * @details 该函数在写入显存之前调用，通常用于设置显示位置。
 */
void LcdDisplay::WriteRAM_Prepare() {
    WR_REG(lcddev.wramcmd);
}

u16 LcdDisplay::ReadRAM() {
    return 0;
}

u16 LcdDisplay::BGR2RGB(u16 c) {
    return 0;
}

void LcdDisplay::SetParam() {
}

/**
 * @brief 设置LCD显示方向
 * @details 0-0度, 1-90度, 2-180度, 3-270度
 */
void LcdDisplay::SetDirection(u8 direction) {
    lcddev.setxcmd = 0x2A; // 对应列地址设置命令
    lcddev.setycmd = 0x2B; // 对应行地址设置命令
    lcddev.wramcmd = 0x2C; // 对应显存写入命令
    switch (direction) {
    case 0:
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        WriteReg(0x36, (1 << 3) | (0 << 6) | (0 << 7)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 1:
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    case 2:
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 7)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 3:
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    default:
        break;
    }
}

/**
 * @brief 向LCD寄存器写入数据
 * @param LCD_Reg: 寄存器地址
 * @param LCD_RegValue: 要写入的值
 * @details 该函数将指定的寄存器地址和对应的值写入LCD控制器，通常用于配置显示参数。
 */
void LcdDisplay::WriteReg(u8 LCD_Reg, u16 LCD_RegValue) {
    WR_REG(LCD_Reg);
    WR_DATA(LCD_RegValue);
}

void LcdDisplay::WriteRAM(u16 RGB_Code) {
    WriteRAM(RGB_Code);
}

/**
 * @brief 在指定位置绘制一个点
 * @param x: 点的x坐标
 * @param y: 点的y坐标
 * @details 该函数在指定的x, y坐标位置绘制一个点，通常用于显示光标或特定图形元素。
 */
void LcdDisplay::DrawPoint(u16 x, u16 y) {
    SetCursor(x, y); // 设置显示位置
    WriteData_16Bit(POINT_COLOR);
}

/**
 * @brief 清除LCD屏幕
 * @param Color: 清除颜色
 */
void LcdDisplay::Clear(u16 Color) {
    unsigned int i, m;
    SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); // 设置显示窗口为全屏范围
    // DISABLE_INT();
    LCD_CS_CLR();
    LCD_DC_SET();
    for (i = 0; i < lcddev.height; i++) {
        for (m = 0; m < lcddev.width; m++) {
            spi_bus_.Write0((Color >> 8) & 0xF8);
            spi_bus_.Write0((Color >> 3) & 0xFC);
            spi_bus_.Write0(Color << 3);
        }
    }
    LCD_CS_SET();
    // ENABLE_INT();
}

/**
 * @brief 设置LCD显示窗口
 * @param xStar: 窗口起始x坐标
 * @param yStar: 窗口起始y坐标
 * @param xEnd: 窗口结束x坐标
 * @param yEnd: 窗口结束y坐标
 * @details 该函数设置LCD的显示窗口，通常用于指定绘图区域。
 */
void LcdDisplay::SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd) {
    WR_REG(lcddev.setxcmd); // 列地址设置命令
    WR_DATA(xStar >> 8);    // 写入16位坐标x（高8位+低8位）
    WR_DATA(0x00FF & xStar);
    WR_DATA(xEnd >> 8);
    WR_DATA(0x00FF & xEnd);

    WR_REG(lcddev.setycmd); // 行地址设置命令
    WR_DATA(yStar >> 8);    // 写入16位坐标y（高8位+低8位）
    WR_DATA(0x00FF & yStar);
    WR_DATA(yEnd >> 8);
    WR_DATA(0x00FF & yEnd);

    WriteRAM_Prepare(); // 开始写入GRAM
}

/**
 * @brief 设置1x1像素的显示窗口
 * @param Xpos: x坐标
 * @param Ypos: y坐标
 * @details 该函数将显示窗口设置为单个像素点，通常用于设置光标位置。
 */
void LcdDisplay::SetCursor(u16 Xpos, u16 Ypos) {
    SetWindows(Xpos, Ypos, Xpos, Ypos);
}

/**
 * @brief 写入16位数据到LCD
 * @param Data: 要写入的数据
 * @details 该函数将16位数据写入LCD显示器，通常用于设置像素颜色。
 * @note 数据格式为RGB565（5位红色、6位绿色、5位蓝色），高位红色、中间绿色、低位蓝色。 16bit-->18bit
 */
void LcdDisplay::WriteData_16Bit(u16 Data) {
    // 16Bit	5   6  5    RGB			xxxxx xxxxxx xxxxx   格式为RGB565（5位红+6位绿+5位蓝）
    // WR_DATA((Data>>8)&0xF8);//RED    1111 1000		右移8位获取高5位红色 与0xF8(11111000)掩码确保高5位有效
    // WR_DATA((Data>>3)&0xFC);//GREEN  1111 1100		右移3位获取中间6位绿色 与0xFC(11111100)掩码确保高6位有效
    // WR_DATA(Data<<3);//BLUE												左移3位将低5位蓝色移到高位
    LCD_DC_SET();
    LCD_CS_CLR();
    spi_bus_.Write0((Data >> 8) & 0xF8);
    spi_bus_.Write0((Data >> 3) & 0xFC);
    spi_bus_.Write0(Data << 3);
    LCD_CS_SET();
}