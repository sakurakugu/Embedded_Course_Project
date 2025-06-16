#include "bsp_gui.h"
#include "lcd_display.h"

// 构造函数
Gui::Gui() {
    // 初始化代码
    display = nullptr;
}

// 析构函数
Gui::~Gui() {
    // 清理代码
}

/**
 * @brief 绘制一个点
 * @param x: 点的x坐标
 * @param y: 点的y坐标
 * @param color: 点的颜色值
 * @details 该函数在LCD屏幕上绘制一个指定颜色的像素点。
 */
void Gui::DrawPoint(u16 x, u16 y, u16 color) {
    SetCursor(x, y); // 设置光标位置
    SetPosWriteData_16Bit(color);
}

/**
 * @brief 填充指定区域
 * @param sx: 填充区域的起始x坐标
 * @param sy: 填充区域的起始y坐标
 * @param ex: 填充区域的结束x坐标
 * @param ey: 填充区域的结束y坐标
 * @param color: 填充颜色值
 * @details 该函数在LCD屏幕上填充一个矩形区域，使用指定的颜色值。
 */
void Gui::Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color) {
    u16 i, j;
    u16 width = ex - sx + 1;        // 得到填充的宽度
    u16 height = ey - sy + 1;       // 高度
    SetPosSetWindows(sx, sy, ex, ey); // 设置显示窗口
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++)
            SetPosWriteData_16Bit(color); // 写入数据
    }
    SetPosSetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); // 恢复窗口设置为全屏
}

/**
 * @brief 绘制一条直线
 * @param x1: 直线起始点的x坐标
 * @param y1: 直线起始点的y坐标
 * @param x2: 直线结束点的x坐标
 * @param y2: 直线结束点的y坐标
 * @details 该函数在LCD屏幕上绘制一条直线
 */
void Gui::DrawLine(u16 x1, u16 y1, u16 x2, u16 y2) {
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t++) // 画线输出
    {
        SetPosDrawPoint(uRow, uCol); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/**
 * @brief 绘制一个矩形
 * @param x1: 矩形左上角的x坐标
 * @param y1: 矩形左上角的y坐标
 * @param x2: 矩形右下角的x坐标
 * @param y2: 矩形右下角的y坐标
 * @details 该函数在LCD屏幕上绘制一个矩形边框。
 *          矩形的边框由四条直线组成。
 */
void Gui::DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2) {
    SetPosDrawLine(x1, y1, x2, y1);
    SetPosDrawLine(x1, y1, x1, y2);
    SetPosDrawLine(x1, y2, x2, y2);
    SetPosDrawLine(x2, y1, x2, y2);
}

/**
 * @brief 填充一个矩形区域
 * @param x1: 矩形左上角的x坐标
 * @param y1: 矩形左上角的y坐标
 * @param x2: 矩形右下角的x坐标
 * @param y2: 矩形右下角的y坐标
 * @details 该函数在LCD屏幕上填充一个矩形区域，使用当前设置的点颜色。
 */
void Gui::DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2) {
    SetPosFill(x1, y1, x2, y2, POINT_COLOR);
}

/**
 * @brief 绘制一个圆
 * @param xc: 圆心的x坐标
 * @param yc: 圆心的y坐标
 * @param x: 圆上点的x坐标
 * @param y: 圆上点的y坐标
 * @param c: 圆的颜色值
 * @details 该函数在LCD屏幕上绘制一个圆的八个对称点。
 *          通过对称点的方式减少绘制点的数量，提高绘制效率。
 */
void Gui::DrawCircle(int xc, int yc, int x, int y, u16 c) {
    GUI_DrawPoint(xc + x, yc + y, c);
    GUI_DrawPoint(xc - x, yc + y, c);
    GUI_DrawPoint(xc + x, yc - y, c);
    GUI_DrawPoint(xc - x, yc - y, c);
    GUI_DrawPoint(xc + y, yc + x, c);
    GUI_DrawPoint(xc - y, yc + x, c);
    GUI_DrawPoint(xc + y, yc - x, c);
    GUI_DrawPoint(xc - y, yc - x, c);
}

/**
 * @brief 绘制一个圆
 * @param xc: 圆心的x坐标
 * @param yc: 圆心的y坐标
 * @param c: 圆的颜色值
 * @param r: 圆的半径
 * @param fill: 是否填充圆（1为填充，0为不填充）
 */
void Gui::Circle(int xc, int yc, u16 c, int r, int fill) {
    int x = 0, y = r, yi, d;
    d = 3 - 2 * r;
    if (fill) {
        // 如果填充（画实心圆）
        while (x <= y) {
            for (yi = x; yi <= y; yi++)
                DrawCircle(xc, yc, x, yi, c);

            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    } else {
        // 如果不填充（画空心圆）
        while (x <= y) {
            DrawCircle(xc, yc, x, y, c);
            if (d < 0) {
                d = d + 4 * x + 6;
            } else {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}

/**
 * @brief 绘制一个三角形
 * @param x0: 三角形第一个顶点的x坐标
 * @param y0: 三角形第一个顶点的y坐标
 * @param x1: 三角形第二个顶点的x坐标
 * @param y1: 三角形第二个顶点的y坐标
 * @param x2: 三角形第三个顶点的x坐标
 * @param y2: 三角形第三个顶点的y坐标
 * @details 该函数在LCD屏幕上绘制一个三角形的边框。
 */
void Gui::DrawTriangel(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2) {
    SetPosDrawLine(x0, y0, x1, y1);
    SetPosDrawLine(x1, y1, x2, y2);
    SetPosDrawLine(x2, y2, x0, y0);
}

/**
 * @brief 填充一个三角形
 * @param x0: 三角形第一个顶点的x坐标
 * @param y0: 三角形第一个顶点的y坐标
 * @param x1: 三角形第二个顶点的x坐标
 * @param y1: 三角形第二个顶点的y坐标
 * @param x2: 三角形第三个顶点的x坐标
 * @param y2: 三角形第三个顶点的y坐标
 * @details 该函数在LCD屏幕上填充一个三角形区域，使用当前设置的点颜色。
 */
void Gui::FillTriangel(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2) {
    u16 a, b, y, last;
    int dx01, dy01, dx02, dy02, dx12, dy12;
    long sa = 0;
    long sb = 0;
    if (y0 > y1) {
        _swap(&y0, &y1);
        _swap(&x0, &x1);
    }
    if (y1 > y2) {
        _swap(&y2, &y1);
        _swap(&x2, &x1);
    }
    if (y0 > y1) {
        _swap(&y0, &y1);
        _swap(&x0, &x1);
    }
    if (y0 == y2) {
        a = b = x0;
        if (x1 < a) {
            a = x1;
        } else if (x1 > b) {
            b = x1;
        }
        if (x2 < a) {
            a = x2;
        } else if (x2 > b) {
            b = x2;
        }
        SetPosFill(a, y0, b, y0, POINT_COLOR);
        return;
    }
    dx01 = x1 - x0;
    dy01 = y1 - y0;
    dx02 = x2 - x0;
    dy02 = y2 - y0;
    dx12 = x2 - x1;
    dy12 = y2 - y1;

    if (y1 == y2) {
        last = y1;
    } else {
        last = y1 - 1;
    }
    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if (a > b) {
            _swap(&a, &b);
        }
        SetPosFill(a, y, b, y, POINT_COLOR);
    }
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (a > b) {
            _swap(&a, &b);
        }
        SetPosFill(a, y, b, y, POINT_COLOR);
    }
}

/**
 * @brief 显示一个字符
 * @param x: 字符的x坐标
 * @param y: 字符的y坐标
 * @param fc: 字符的前景色
 * @param bc: 字符的背景色
 * @param num: 要显示的字符（ASCII码）
 * @param size: 字符的大小（12或16）
 * @param mode: 显示模式（0-不叠加，1-叠加）
 */
void Gui::ShowChar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode) {
    u8 temp;
    u8 pos, t;
    u16 colortemp = POINT_COLOR;

    num = num - ' ';                                      // 得到偏移后的值
    SetPosSetWindows(x, y, x + size / 2 - 1, y + size - 1); // 设置单个文字显示窗口
    if (!mode)                                            // 非叠加方式
    {
        for (pos = 0; pos < size; pos++) {
            if (size == 12)
                temp = asc2_1206[num][pos]; // 调用1206字体
            else
                temp = asc2_1608[num][pos]; // 调用1608字体
            for (t = 0; t < size / 2; t++) {
                if (temp & 0x01)
                    SetPosWriteData_16Bit(fc);
                else
                    SetPosWriteData_16Bit(bc);
                temp >>= 1;
            }
        }
    } else // 叠加方式
    {
        for (pos = 0; pos < size; pos++) {
            if (size == 12)
                temp = asc2_1206[num][pos]; // 调用1206字体
            else
                temp = asc2_1608[num][pos]; // 调用1608字体
            for (t = 0; t < size / 2; t++) {
                POINT_COLOR = fc;
                if (temp & 0x01)
                    SetPosDrawPoint(x + t, y + pos); // 画一个点
                temp >>= 1;
            }
        }
    }
    POINT_COLOR = colortemp;
    SetPosSetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); // 恢复窗口为全屏
}

/**
 * @brief 显示一个数字
 * @param x: 数字的起始x坐标
 * @param y: 数字的起始y坐标
 * @param num: 要显示的数字
 * @param len: 数字的长度（位数）
 * @param size: 数字的大小（12或16）
 * @details 该函数在LCD屏幕上显示一个数字，支持指定长度和大小。
 */
void Gui::ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size) {
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                SetPosShowChar(x + (size / 2) * t, y, POINT_COLOR, BACK_COLOR, ' ', size, 0);
                continue;
            } else
                enshow = 1;
        }
        SetPosShowChar(x + (size / 2) * t, y, POINT_COLOR, BACK_COLOR, temp + '0', size, 0);
    }
}

/**
 * @brief 显示一个字符串
 * @param x: 字符串的起始x坐标
 * @param y: 字符串的起始y坐标
 * @param size: 字符串的字符大小
 * @param p: 指向字符串的指针
 * @param mode: 显示模式（0-不叠加，1-叠加）
 * @details 该函数在LCD屏幕上显示一个字符串，直到遇到非法字符（非ASCII可打印字符）。
 */
void Gui::ShowString(u16 x, u16 y, u8 size, u8 *p, u8 mode) {
    while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
    {
        if (x > (lcddev.width - 1) || y > (lcddev.height - 1))
            return;
        SetPosShowChar(x, y, POINT_COLOR, BACK_COLOR, *p, size, mode);
        x += size / 2;
        p++;
    }
}

/**
 * @brief 绘制一个16x16的汉字
 * @param x: 汉字的起始x坐标
 * @param y: 汉字的起始y坐标
 * @param fc: 汉字的前景色
 * @param bc: 汉字的背景色
 * @param s: 指向汉字的指针（4字节，UTF-8编码）
 * @param mode: 显示模式（0-不叠加，1-叠加）
 * @details 该函数在LCD屏幕上显示一个16x16的汉字，使用UTF-8编码。
 */
void Gui::DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode) {
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); // 自动统计汉字数目

    for (k = 0; k < HZnum; k++) {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)) && (tfont16[k].Index[2] == *(s + 2))) {
            SetPosSetWindows(x, y, x + 16 - 1, y + 16 - 1);
            for (i = 0; i < 16 * 2; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x80 >> j))
                            SetPosWriteData_16Bit(fc);
                        else
                            SetPosWriteData_16Bit(bc);
                    } else {
                        POINT_COLOR = fc;
                        if (tfont16[k].Msk[i] & (0x80 >> j))
                            SetPosDrawPoint(x, y); // 画一个点
                        x++;
                        if ((x - x0) == 16) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    SetPosSetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); // 恢复窗口为全屏
}

/**
 * @brief 绘制一个24x24的汉字
 * @param x: 汉字的起始x坐标
 * @param y: 汉字的起始y坐标
 * @param fc: 汉字的前景色
 * @param bc: 汉字的背景色
 * @param s: 指向汉字的指针（4字节，UTF-8编码）
 * @param mode: 显示模式（0-不叠加，1-叠加）
 * @details 该函数在LCD屏幕上显示一个24x24的汉字，使用UTF-8编码。
 */
void Gui::DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode) {
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); // 自动统计汉字数目

    for (k = 0; k < HZnum; k++) {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)) && (tfont24[k].Index[2] == *(s + 2))) {
            SetPosSetWindows(x, y, x + 24 - 1, y + 24 - 1);
            for (i = 0; i < 24 * 3; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x80 >> j))
                            SetPosWriteData_16Bit(fc);
                        else
                            SetPosWriteData_16Bit(bc);
                    } else {
                        POINT_COLOR = fc;
                        if (tfont24[k].Msk[i] & (0x80 >> j))
                            SetPosDrawPoint(x, y); // 画一个点
                        x++;
                        if ((x - x0) == 24) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    SetPosSetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); // 恢复窗口为全屏
}

/**
 * @brief 绘制一个32x32的汉字
 * @param x: 汉字的起始x坐标
 * @param y: 汉字的起始y坐标
 * @param fc: 汉字的前景色
 * @param bc: 汉字的背景色
 * @param s: 指向汉字的指针（4字节，UTF-8编码）
 * @param mode: 显示模式（0-不叠加，1-叠加）
 * @details 该函数在LCD屏幕上显示一个32x32的汉字，使用UTF-8编码。
 */
void Gui::DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode) {
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); // 自动统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)) && (tfont32[k].Index[2] == *(s + 2))) {
            SetPosSetWindows(x, y, x + 32 - 1, y + 32 - 1);
            for (i = 0; i < 32 * 4; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x80 >> j))
                            SetPosWriteData_16Bit(fc);
                        else
                            SetPosWriteData_16Bit(bc);
                    } else {
                        POINT_COLOR = fc;
                        if (tfont32[k].Msk[i] & (0x80 >> j))
                            SetPosDrawPoint(x, y); // 画一个点
                        x++;
                        if ((x - x0) == 32) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    SetPosSetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); // 恢复窗口为全屏
}

/**
 * @brief 显示一个字符串
 * @param x: 字符串的起始x坐标
 * @param y: 字符串的起始y坐标
 * @param fc: 字符串的前景色
 * @param bc: 字符串的背景色
 * @param str: 指向字符串的指针（UTF-8编码）
 * @param size: 字符串的字符大小（12、16、24或32）
 * @param mode: 显示模式（0-不叠加，1-叠加）
 * @details 该函数在LCD屏幕上显示一个字符串，支持中文和英文字符。
 */
void Gui::ShowStr(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode) {
    u16 x0 = x;
    u8 bHz = 0;       // 字符或者中文
    while (*str != 0) // 数据未结束
    {
        if (!bHz) {
            if (x > (lcddev.width - size / 2) || y > (lcddev.height - size))
                return;
            if (*str > 0x80)
                bHz = 1; // 中文
            else         // 字符
            {
                if (*str == 0x0D) // 换行符号
                {
                    y += size;
                    x = x0;
                    str++;
                } else {
                    if (size > 16) // 字库中没有集成12X24 16X32的英文字体,用8X16代替
                    {
                        SetPosShowChar(x, y, fc, bc, *str, 16, mode);
                        x += 8; // 字符,为全字的一半
                    } else {
                        SetPosShowChar(x, y, fc, bc, *str, size, mode);
                        x += size / 2; // 字符,为全字的一半
                    }
                }
                str++;
            }
        } else // 中文
        {
            if (x > (lcddev.width - size) || y > (lcddev.height - size))
                return;
            bHz = 0; // 有汉字库
            if (size == 32)
                GUI_DrawFont32(x, y, fc, bc, str, mode);
            else if (size == 24)
                GUI_DrawFont24(x, y, fc, bc, str, mode);
            else
                GUI_DrawFont16(x, y, fc, bc, str, mode);

            str += 3;
            x += size; // 下一个汉字偏移
        }
    }
}

/**
 * @brief 居中显示字符串
 * @param x: 起始x坐标（未使用）
 * @param y: 起始y坐标
 * @param fc: 前景色
 * @param bc: 背景色
 * @param str: 指向字符串的指针（UTF-8编码）
 * @param size: 字符串的字符大小（12、16、24或32）
 * @param mode: 显示模式（0-不叠加，1-叠加）
 * @details 该函数在LCD屏幕上居中显示一个字符串。
 *          字符串会根据屏幕宽度自动计算起始x坐标，使其居中显示。
 */
void Gui::StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode) {
    u16 len = strlen((const char *)str);
    u16 x1 = (lcddev.width - len * 8) / 2;
    Show_Str(x1, y, fc, bc, str, size, mode);
}

/**
 * @brief 绘制16位BMP图像
 * @param x: 图像的起始x坐标
 * @param y: 图像的起始y坐标
 * @param bmp_x: 图像的宽度
 * @param bmp_y: 图像的高度
 * @param p: 指向图像数据的指针
 * @details 该函数在LCD屏幕上绘制一个16位BMP图像。
 */
void Gui::Drawbmp16(u16 x, u16 y, uint16_t bmp_x, uint16_t bmp_y, const unsigned char *p) {
    uint64_t i;
    unsigned char picH, picL;
    SetPosSetWindows(x, y, x + bmp_x - 1, y + bmp_y - 1); // 窗口设置
    for (i = 0; i < (bmp_x * bmp_y); i++) {
        picL = *(p + i * 2); // 数据低位在前
        picH = *(p + i * 2 + 1);
        SetPosWriteData_16Bit(picH << 8 | picL);
    }
    SetPosSetWindows(0, 0, lcddev.width - 1, lcddev.height - 1); // 恢复显示窗口为全屏
}
