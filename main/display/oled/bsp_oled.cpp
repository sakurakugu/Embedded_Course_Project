#include "bsp_oled.h"

void oledWriteByte(uint8_t dat, uint8_t cmd) {
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

// x:0-127,y:0-7，但是在实际显示字符或汉字时，x的取值要相应减少字模占的列数
void oled_SetPos(unsigned char x, unsigned char y) {
    oledWriteByte(0xb0 + y,
                  OLED_CMD); // 页地址模式P31,Set GDDRAM Page Start Address(PAGE0~PAGE7) for Page Addressing Mode
    oledWriteByte(((x & 0xf0) >> 4) | 0x10, OLED_CMD); // P30高起始列地址
    oledWriteByte((x & 0x0f) | 0x00, OLED_CMD);        // P30低起始列地址
}

void oled_DisplayOn(void) {
    oledWriteByte(0X8D, OLED_CMD); // SET DCDC
    oledWriteByte(0X14, OLED_CMD); // DCDC ON
    oledWriteByte(0XAF, OLED_CMD); // DISPLAY ON
}

void oled_DisplayOff(void) {
    oledWriteByte(0X8D, OLED_CMD); // SET DCDC
    oledWriteByte(0X10, OLED_CMD); // DCDC OFF
    oledWriteByte(0XAE, OLED_CMD); // DISPLAY OFF
}
// 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void oled_Clear(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        oledWriteByte(0xb0 + i, OLED_CMD);  // 设置页地址（0~7）P31
        oledWriteByte(0x00, OLED_CMD);      // 设置显示位置—列低地址 P30
        oledWriteByte(0x10, OLED_CMD);      // 设置显示位置—列高地址 P30
        for (n = 0; n < 128; n++)           // 由于是页地址模式，所以只能写这一页，128个字节，然后换页
            oledWriteByte(0x00, OLED_DATA); // 0x00全不亮，0xff全亮， 一个字节对应8个点
    } // 更新显示
}

// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~7
// mode:0,反白显示;1,正常显示
// size:选择字体 16/8

void oled_ShowChar(uint8_t x, uint8_t y, uint8_t chr) {
    unsigned char c = 0, i = 0;
    c = chr - ' '; // 得到偏移后的值
    // 如果设定的X值大于127时，在下一行显示这个字符
    if (x > Max_Column - 1) {
        x = 0;
        y = y + 2;
    }
    if (OLED_SIZE == 16) { // 8*16字模
        oled_SetPos(x, y);
        for (i = 0; i < 8; i++)
            oledWriteByte(F8X16[c * 16 + i], OLED_DATA);
        oled_SetPos(x, y + 1); // 显示字体的下半部分，Y坐标+1换行
        for (i = 0; i < 8; i++)
            oledWriteByte(F8X16[c * 16 + i + 8], OLED_DATA);
    } else { // 6*8字模
        oled_SetPos(x, y + 1);
        for (i = 0; i < 6; i++) {
            // oledSetPos(x+i, y + 1);
            oledWriteByte(F6x8[c][i], OLED_DATA);
        }
    }
}
// m^n函数
uint32_t oled_pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}
// 显示2个数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:模式	0,填充模式;1,叠加模式
// num:数值(0~4294967295);

void oled_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2) { // 不支持自动换行
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10; // 取数值的各位数
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                oled_ShowChar(x + (size2 / 2) * t, y, '0'); // 当len长度大于num的位数时，前面填充0而不是空白
                continue;
            } else
                enshow = 1;
        }
        oled_ShowChar(x + (size2 / 2) * t, y, temp + '0');
    }
}

void oled_ShowAsciiString(uint8_t x, uint8_t y, uint8_t *ch) { //(0,0."hello world")
    unsigned char j = 0;
    while (ch[j] != '\0') {
        oled_ShowChar(x, y, ch[j]);
        x += 8;      // 写完一个字符后，地址加
        if (x > 120) // 换行
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

// oled_ShowChinese(0,0,"我们嵌入式");
void oled_ShowChinese(unsigned char X, unsigned char Y, unsigned char *pstr) {
    unsigned char i;
    unsigned char Addr = 0;
    unsigned char count = 0;
    // oled_SetPos(X,Y);                             //设置初始位置
    while (*pstr) //*pstr操作是返回p的值作为地址的那个空间的取值
    {
        for (i = 0; i < chinese_12_len; i++) // 在字库中根据汉字GBK内码寻找汉字的字模
        {
            if ((*pstr == chinese_12[i].value[0]) && (*(pstr + 1) == chinese_12[i].value[1]) &&
                (*(pstr + 2) == chinese_12[i].value[2]) && (*(pstr + 3) == chinese_12[i].value[3])) {
                Addr = i; // Addr 汉字在字库中的下标
                break;
            }
        }
        oled_SetPos(X + count * 12, Y); // 设置初始位置
        for (i = 0; i < 12; i++)        // 写上半部分
        {
            oledWriteByte(chinese_12[Addr].data[i], 1);
        }
        oled_SetPos(X + count * 12, Y + 1); // 写汉字的下半部分，更改Y坐标的值
        for (i = 0; i < 12; i++)            // 写一个汉字
        {
            oledWriteByte(chinese_12[Addr].data[12 + i], 1);
        }
        count++; // 已显示的字数
        pstr = pstr + 3;
    }
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(X,Y),x的范围0～127，y为页的范围0～7
Pix_x Pix_y是指位图的宽和高*****************/
void oled_DrawBMP(unsigned char X, unsigned char Y, unsigned char Pix_x, unsigned char Pix_y,
                  const unsigned char *BMP) {
    unsigned int i, n;
    unsigned char row;

    if (Pix_y % 8 == 0)
        row = Pix_y / 8; // 计算位图所占行数
    else
        row = Pix_y / 8 + 1;

    for (n = 0; n < row; n++) {
        oled_SetPos(X, Y);
        for (i = 0; i < Pix_x; i++) {
            oledWriteByte(BMP[i + n * Pix_x], OLED_DATA);
        }
        Y++; // 换行
    }
}

void OLED_gpioInit(void) {
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

void oled_Init(void) {

    OLED_gpioInit();
    OLED_RST_SET();
    OLED_RST_CLR();
    SysTick_Delay_Ms(100);
    OLED_RST_SET();

    oledWriteByte(0xAE, OLED_CMD); //--turn off oled panel
    oledWriteByte(0x00, OLED_CMD); //---set low column address
    oledWriteByte(0x10, OLED_CMD); //---set high column address
    oledWriteByte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oledWriteByte(0x81, OLED_CMD); //--set contrast control register
    oledWriteByte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    oledWriteByte(0xA1, OLED_CMD); //--Set SEG/Column Mapping      0xa0左右反置 0xa1正常
    oledWriteByte(0xC8, OLED_CMD); // Set COM/Row Scan Direction    0xc0上下反置 0xc8正常
    oledWriteByte(0xA6, OLED_CMD); //--set normal display
    oledWriteByte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    oledWriteByte(0x3f, OLED_CMD); //--1/64 duty
    oledWriteByte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    oledWriteByte(0x00, OLED_CMD); //-not offset
    oledWriteByte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    oledWriteByte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    oledWriteByte(0xD9, OLED_CMD); //--set pre-charge period
    oledWriteByte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oledWriteByte(0xDA, OLED_CMD); //--set com pins hardware configuration
    oledWriteByte(0x12, OLED_CMD);
    oledWriteByte(0xDB, OLED_CMD); //--set vcomh
    oledWriteByte(0x40, OLED_CMD); // Set VCOM Deselect Level
    oledWriteByte(0x20, OLED_CMD); //-Set Memory Addressing Mode (0x00/0x01/0x02)
    oledWriteByte(0x02, OLED_CMD); // Mode 02	Page Addressing Mode
    oledWriteByte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
    oledWriteByte(0x14, OLED_CMD); //--set(0x10) disable
    oledWriteByte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    oledWriteByte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
    oledWriteByte(0xAF, OLED_CMD); //--turn on oled panel

    oledWriteByte(0xAF, OLED_CMD); /*display ON*/
    oled_Clear();
    oled_SetPos(0, 0);
}

// 显示中英文混合文本
// X,Y: 起始坐标
// str: 字符串，UTF-8编码
void oled_ShowString(unsigned char X, unsigned char Y, unsigned char *str) {
    unsigned char i;
    unsigned char x = X;
    unsigned char y = Y;
    unsigned char Addr = 0;
    unsigned char buffer[4] = {0}; // 临时缓冲区
    // unsigned char count = 0;

    while (*str) {
        if (*str < 0x80) { // ASCII字符
            oled_ShowChar(x, y, *str);
            x += 8;        // ASCII字符宽度为8
            if (x > 120) { // 需要换行
                x = 0;
                y += 2;
            }
            str++;
        } else { // 中文字符
            buffer[0] = *str;
            buffer[1] = *(str + 1);
            buffer[2] = *(str + 2);
            buffer[3] = '\0';

            // 在字库中查找汉字
            for (i = 0; i < chinese_12_len; i++) {
                if ((buffer[0] == chinese_12[i].value[0]) && (buffer[1] == chinese_12[i].value[1]) &&
                    (buffer[2] == chinese_12[i].value[2]) && (buffer[3] == chinese_12[i].value[3])) {
                    Addr = i;
                    break;
                }
            }

            // 显示汉字
            oled_SetPos(x, y);
            for (i = 0; i < 12; i++) { // 写上半部分
                oledWriteByte(chinese_12[Addr].data[i], 1);
            }
            oled_SetPos(x, y + 1); // 写汉字的下半部分
            for (i = 0; i < 12; i++) {
                oledWriteByte(chinese_12[Addr].data[12 + i], 1);
            }

            x += 12;       // 汉字宽度为12
            if (x > 116) { // 需要换行
                x = 0;
                y += 2;
            }
            str += 3; // UTF-8中文占3个字节
        }
    }
}
