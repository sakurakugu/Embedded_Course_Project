#pragma once

#include "bsp.h"
#include "display.h"

// 声明GUI类
class Gui {
  public:
    // 构造和析构函数
    Gui();
    ~Gui();

    // 单例模式实现
    static Gui &GetInstance() {
        static Gui instance;
        return instance;
    }

    // 禁止复制
    Gui(const Gui &) = delete;
    Gui &operator=(const Gui &) = delete;

    // GUI绘图函数
    void DrawPoint(u16 x, u16 y, u16 color);
    void Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);
    void DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
    void DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
    void DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
    void DrawCircle(int xc, int yc, int x, int y, u16 c);
    void Circle(int xc, int yc, u16 c, int r, int fill);
    void DrawTriangel(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2);
    void FillTriangel(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2);

    // 文字显示函数
    void ShowChar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode);
    void ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size);
    void ShowString(u16 x, u16 y, u8 size, u8 *p, u8 mode);
    void DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
    void DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
    void DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
    void ShowStr(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode);
    void StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode);

    // 图像显示函数
    void Drawbmp16(u16 x, u16 y, uint16_t bmp_x, uint16_t bmp_y, const unsigned char *p);

  private:
    // 可能需要的私有成员变量
    Display *display;
};
