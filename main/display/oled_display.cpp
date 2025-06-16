#include "oled_display.h"
#include "bsp_oled.h"

OledDisplay::OledDisplay() {
}

OledDisplay::~OledDisplay() {
    // 释放资源或进行必要的清理
}

void OledDisplay::DisplayOn() {
    WriteByte(0X8D, OLED_CMD); // SET DCDC
    WriteByte(0X14, OLED_CMD); // DCDC ON
    WriteByte(0XAF, OLED_CMD); // DISPLAY ON
}

void OledDisplay::DisplayOff() {
    WriteByte(0X8D, OLED_CMD); // SET DCDC
    WriteByte(0X10, OLED_CMD); // DCDC OFF
    WriteByte(0XAE, OLED_CMD); // DISPLAY OFF
}

void OledDisplay::GpioInit() {
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 打开GPIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出模式 */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void OledDisplay::Init() {
    OLED_gpioInit();
    OLED_RST_SET();
    OLED_RST_CLR();
    SysTick_Delay_Ms(100);
    OLED_RST_SET();

    WriteByte(0xAE, OLED_CMD); //--turn off oled panel
    WriteByte(0x00, OLED_CMD); //---set low column address
    WriteByte(0x10, OLED_CMD); //---set high column address
    WriteByte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    WriteByte(0x81, OLED_CMD); //--set contrast control register
    WriteByte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    WriteByte(0xA1, OLED_CMD); //--Set SEG/Column Mapping      0xa0左右反置 0xa1正常
    WriteByte(0xC8, OLED_CMD); // Set COM/Row Scan Direction    0xc0上下反置 0xc8正常
    WriteByte(0xA6, OLED_CMD); //--set normal display
    WriteByte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    WriteByte(0x3f, OLED_CMD); //--1/64 duty
    WriteByte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    WriteByte(0x00, OLED_CMD); //-not offset
    WriteByte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    WriteByte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    WriteByte(0xD9, OLED_CMD); //--set pre-charge period
    WriteByte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    WriteByte(0xDA, OLED_CMD); //--set com pins hardware configuration
    WriteByte(0x12, OLED_CMD);
    WriteByte(0xDB, OLED_CMD); //--set vcomh
    WriteByte(0x40, OLED_CMD); // Set VCOM Deselect Level
    WriteByte(0x20, OLED_CMD); //-Set Memory Addressing Mode (0x00/0x01/0x02)
    WriteByte(0x02, OLED_CMD); // Mode 02	Page Addressing Mode
    WriteByte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
    WriteByte(0x14, OLED_CMD); //--set(0x10) disable
    WriteByte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    WriteByte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
    WriteByte(0xAF, OLED_CMD); //--turn on oled panel

    WriteByte(0xAF, OLED_CMD); /*display ON*/
    oled_Clear();
    SetPos(0, 0);
}

void OledDisplay::Clear() {
    u8 i, n;
    for (i = 0; i < 8; i++) {
        WriteByte(0xb0 + i, OLED_CMD);  // 设置页地址（0~7）P31
        WriteByte(0x00, OLED_CMD);      // 设置显示位置—列低地址 P30
        WriteByte(0x10, OLED_CMD);      // 设置显示位置—列高地址 P30
        for (n = 0; n < 128; n++)       // 由于是页地址模式，所以只能写这一页，128个字节，然后换页
            WriteByte(0x00, OLED_DATA); // 0x00全不亮，0xff全亮， 一个字节对应8个点
    } // 更新显示
}

void OledDisplay::DrawPoint(uint8_t x, uint8_t y, uint8_t t) {
}

void OledDisplay::Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot) {
}

/**
 * @brief 在指定位置显示一个字符
 * @param x: 字符的x坐标
 * @param y: 字符的y坐标
 * @param ch: 要显示的字符
 * @details 该函数在指定的x, y坐标位置显示一个字符，支持8*16或6*8字模。
 */
void OledDisplay::ShowChar(uint8_t x, uint8_t y, uint8_t ch) {
    unsigned char c = 0, i = 0;
    c = ch - ' '; // 得到偏移后的值
    // 如果设定的X值大于127时，在下一行显示这个字符
    if (x > Max_Column - 1) {
        x = 0;
        y = y + 2;
    }
    if (SIZE == 16) // 8*16字模
    {
        SetPos(x, y);
        for (i = 0; i < 8; i++)
            WriteByte(F8X16[c * 16 + i], OLED_DATA);
        SetPos(x, y + 1); // 显示字体的下半部分，Y坐标+1换行
        for (i = 0; i < 8; i++)
            WriteByte(F8X16[c * 16 + i + 8], OLED_DATA);
    } else // 6*8字模
    {
        SetPos(x, y + 1);
        for (i = 0; i < 6; i++) {
            // oledSetPos(x+i, y + 1);
            WriteByte(F6x8[c][i], OLED_DATA);
        }
    }
}

void OledDisplay::WriteByte(u8 dat, u8 cmd) {
    if (cmd) {
        OLED_DC_SET();
    } else if (cmd != 1) {
        OLED_DC_CLR();
    }
    OLED_CS_CLR();
    // DISABLE_INT();
    bsp_spiWrite0(dat);
    OLED_CS_SET();
    // ENABLE_INT();
    OLED_DC_SET();
}

uint32_t OledDisplay::Pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result; // 返回m的n次方
}

/**
 * @brief 显示2个数字
 * @param x: 起始x坐标
 * @param y: 起始y坐标
 * @param num: 要显示的数字，数值(0~4294967295)
 * @param len: 数字的长度
 * @param size2: 字符的宽度（通常为8或16）
 * @details 该函数将数字显示在指定的坐标位置，支持前导零填充。不支持自动换行
 */
void OledDisplay::ShowNum(uint8_t x, uint8_t y, u32 num, uint8_t len, uint8_t size2) {
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10; // 取数值的各位数
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                oled_ShowChar(x + (size2 / 2) * t, y, ' '); // 当len长度大于num的位数时，前面填充空白
                continue;
            } else
                enshow = 1;
        }
        oled_ShowChar(x + (size2 / 2) * t, y, temp + '0');
    }
}

void OledDisplay::ShowChinese(u8 x, u8 y, u8 no) {
    u8 t, adder = 0;
    SetPos(x, y);
    for (t = 0; t < 16; t++) // 汉字的上半部分
    {
        WriteByte(Hzk[2 * no][t], OLED_DATA);
        // adder += 1;
    }
    SetPos(x, y + 1); // 坐标切换到下一行，显示汉字的下半部分
    for (t = 0; t < 16; t++) {
        WriteByte(Hzk[2 * no + 1][t], OLED_DATA);
        // adder += 1;
    }
}

void OledDisplay::ShowString(uint8_t x, uint8_t y, uint8_t *p) {
    unsigned char j = 0;
    while (chr[j] != '\0') {
        oled_ShowChar(x, y, chr[j]);
        x += 8;      // 写完一个字符后，地址加
        if (x > 120) // 换行
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

/**
 * @brief 设置显示位置
 * @param x: 起始x坐标，范围为0~127
 * @param y: 起始y坐标，范围为0~7
 * @details 该函数设置OLED显示屏的起始位置，通常用于定位后续绘制或显示操作。
 * @note
 * 注意：x坐标范围为0~127，y坐标范围为0~7（对应8页），每页8行。但是在实际显示字符或汉字时，x的取值要相应减少字模占的列数
 */
void OledDisplay::SetPos(unsigned char x, unsigned char y) {
    WriteByte(0xb0 + y,
              OLED_CMD); // 页地址模式P31,Set GDDRAM Page Start Address(PAGE0~PAGE7) for Page Addressing Mode
    WriteByte(((x & 0xf0) >> 4) | 0x10, OLED_CMD); // P30高起始列地址
    WriteByte((x & 0x0f) | 0x00, OLED_CMD);        // P30低起始列地址
}

// ShowCHStr(0,0,"我们嵌入式");
void OledDisplay::ShowCHStr(unsigned char X, unsigned char Y, unsigned char *pstr) {
    unsigned char i;
    unsigned char Addr = 0;
    unsigned char count = 0;
    // SetPos(X,Y);                             //设置初始位置
    while (*pstr) //*pstr操作是返回p的值作为地址的那个空间的取值
    {
        for (i = 0; i < chinese_12_len; i++) // 在字库中根据汉字GBK内码寻找汉字的字模
        {
            if ((*pstr == chinese_12[i].value[0]) && (*(pstr + 1) == chinese_12[i].value[1]) &&
                (*(pstr + 2) == chinese_12[i].value[2])) {
                Addr = i; // Addr 汉字在字库中的下标
                break;
            }
        }
        SetPos(X + count * 12, Y); // 设置初始位置
        for (i = 0; i < 12; i++)   // 写上半部分
        {
            WriteByte(chinese_12[Addr].data[i], 1);
        }
        SetPos(X + count * 12, Y + 1); // 写汉字的下半部分，更改Y坐标的值
        for (i = 0; i < 12; i++)       // 写一个汉字
        {
            WriteByte(chinese_12[Addr].data[12 + i], 1);
        }
        count++; // 已显示的字数
        pstr = pstr + 3;
    }
}

/**
 * @brief 绘制位图
 * @param X: 起始X坐标，x坐标范围为0~127
 * @param Y: 起始Y坐标，y坐标范围为0~63
 * @param Pix_x: 位图宽度（像素）
 * @param Pix_y: 位图高度（像素）
 * @param BMP: 位图数据指针
 * @details 该函数将指定的位图数据绘制到OLED显示屏上，从指定的起始坐标开始。
 */
void OledDisplay::DrawBMP(unsigned char X, unsigned char Y, unsigned char Pix_x, unsigned char Pix_y,
                          const unsigned char *BMP) {
    if (Pix_y % 8 == 0)
        row = Pix_y / 8; // 计算位图所占行数
    else
        row = Pix_y / 8 + 1;

    for (n = 0; n < row; n++) {
        SetPos(X, Y);
        for (i = 0; i < Pix_x; i++) {
            WriteByte(BMP[i + n * Pix_x], OLED_DATA);
        }
        Y++; // 换行
    }
}