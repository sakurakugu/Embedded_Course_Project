#include "touch.h"
#include "gui.h"

// 构造函数
TouchScreen::TouchScreen() {
    // 初始化成员变量
    x0 = 0;
    y0 = 0;
    x = 0;
    y = 0;
    sta = 0;
    xfac = 0;
    yfac = 0;
    xoff = 0;
    yoff = 0;
    touchtype = 0;
    CMD_RDX = 0XD0;
    CMD_RDY = 0X90;
    display = Board::GetInstance().GetLcdDisplay(); // 获取LCD显示实例
    gui = Board::GetInstance().GetGui();            // 获取GUI实例
    Init();                                         // 调用初始化函数
}

// 析构函数
TouchScreen::~TouchScreen() {
    // 清理资源如果有需要
}

// 初始化触摸屏
u8 TouchScreen::Init() {
    // 注意,时钟使能之后,对GPIO的操作才有效
    // 所以上拉之前,必须使能时钟.才能实现真正的上拉输出
    GPIO_InitTypeDef GPIO_InitStructure; // GPIO

    // 注意,时钟使能之后,对GPIO的操作才有效
    // 所以上拉之前,必须使能时钟.才能实现真正的上拉输出
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE, ENABLE);

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_13;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(GPIOC, &GPIO_InitStructure);

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //上拉输入
    // GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    ReadXY(&x, &y); // 第一次读取初始化
    // AT24CXX_Init();//初始化24CXX
    display->Clear(WHITE);
    if (GetAdjData())
        return 0; // 已经校准
    else          // 未校准?
    {
        display->Clear(WHITE); // 清屏
        Adjust();              // 屏幕校准
        SaveAdjData();
    }
    uint32_t nCount = 100000;
    for (; nCount != 0; nCount--) {
    }
    GetAdjData();
    return 1;
}

/**
 * @brief 扫描触摸屏
 * @param tp 扫描模式：0-屏幕扫描，1-物理坐标
 * @return u8 触摸状态
 */
u8 TouchScreen::Scan(u8 tp) {
    if (ReadPEN() == 0) // 有按键按下
    {
        if (tp)
            ReadXY2(&x, &y);      // 读取物理坐标
        else if (ReadXY2(&x, &y)) // 读取屏幕坐标
        {
            x = xfac * x + xoff; // 将结果转换为屏幕坐标
            y = yfac * y + yoff;
        }
        if ((sta & TP_PRES_DOWN) == 0) // 之前没有被按下
        {
            sta = TP_PRES_DOWN | TP_CATH_PRES; // 按键按下
            x0 = x;                            // 记录第一次按下时的坐标
            y0 = y;
        }
    } else {
        if (sta & TP_PRES_DOWN) // 之前是被按下的
        {
            sta &= ~(1 << 7); // 标记按键松开
        } else                // 之前就没有被按下
        {
            x0 = 0;
            y0 = 0;
            x = 0xffff;
            y = 0xffff;
        }
    }
    return sta & TP_PRES_DOWN; // 返回当前的触屏状态
}

// 触摸屏校准
void TouchScreen::Adjust() {
    u16 pos_temp[4][2]; // 坐标缓存值
    u8 cnt = 0;
    u16 d1, d2;
    u32 tem1, tem2;
    float fac;
    u16 outtime = 0;
    cnt = 0;
    display->POINT_COLOR = BLUE;
    display->BACK_COLOR = WHITE;
    display->Clear(WHITE); // 清屏
    display->POINT_COLOR = RED;     // 红色
    display->Clear(WHITE); // 清屏
    display->POINT_COLOR = BLACK;
    gui->ShowString(10, 40, 16, (u8 *)"Please use the stylus click the", 1);    // 显示提示信息
    gui->ShowString(10, 56, 16, (u8 *)"cross on the screen.The cross will", 1); // 显示提示信息
    gui->ShowString(10, 72, 16, (u8 *)"always move until the screen ", 1);      // 显示提示信息
    gui->ShowString(10, 88, 16, (u8 *)"adjustment is completed.", 1);           // 显示提示信息

    DrawTouchPoint(20, 20, RED); // 画点1
    sta = 0;                          // 消除触发信号
    xfac = 0;                         // xfac用来标记是否校准过,所以校准之前必须清掉!以免错误
    while (1)                         // 如果连续10秒钟没有按下,则自动退出
    {
        Scan(1); // 扫描物理坐标
        // printf("The first: %f,%d,%d\n",fac,d1,d2);
        if ((sta & 0xc0) == TP_CATH_PRES) // 按键按下了一次(此时按键松开了.)
        {
            outtime = 0;
            sta &= ~(1 << 6); // 标记按键已经被处理过了.

            pos_temp[cnt][0] = x;
            pos_temp[cnt][1] = y;
            cnt++;
            switch (cnt) {
            case 1:
                DrawTouchPoint(20, 20, WHITE);                       // 清除点1
                DrawTouchPoint(display->lcddev.width - 20, 20, RED); // 画点2
                break;
            case 2:
                DrawTouchPoint(display->lcddev.width - 20, 20, WHITE); // 清除点2
                DrawTouchPoint(20, display->lcddev.height - 20, RED);  // 画点3
                break;
            case 3:
                DrawTouchPoint(20, display->lcddev.height - 20, WHITE);                       // 清除点3
                DrawTouchPoint(display->lcddev.width - 20, display->lcddev.height - 20, RED); // 画点4
                break;
            case 4: // 全部四个点已经得到
                // 对边相等
                tem1 = abs(pos_temp[0][0] - pos_temp[1][0]); // x1-x2
                tem2 = abs(pos_temp[0][1] - pos_temp[1][1]); // y1-y2
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = std::sqrt(tem1 + tem2); // 得到1,2的距离

                tem1 = abs(pos_temp[2][0] - pos_temp[3][0]); // x3-x4
                tem2 = abs(pos_temp[2][1] - pos_temp[3][1]); // y3-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = std::sqrt(tem1 + tem2); // 得到3,4的距离
                fac = (float)d1 / d2;

                // printf("The first: %f,%d,%d\n",fac,d1,d2);

                if (fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0) // 不合格
                {
                    // printf("The first is fail!\n");
                    cnt = 0;
                    DrawTouchPoint(display->lcddev.width - 20, display->lcddev.height - 20, WHITE); // 清除点4
                    DrawTouchPoint(20, 20, RED);                                                    // 画点1
                    ShowAdjInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                                pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); // 显示数据
                    continue;
                }
                tem1 = abs(pos_temp[0][0] - pos_temp[2][0]); // x1-x3
                tem2 = abs(pos_temp[0][1] - pos_temp[2][1]); // y1-y3
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = std::sqrt(tem1 + tem2); // 得到1,3的距离

                tem1 = abs(pos_temp[1][0] - pos_temp[3][0]); // x2-x4
                tem2 = abs(pos_temp[1][1] - pos_temp[3][1]); // y2-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = std::sqrt(tem1 + tem2); // 得到2,4的距离
                fac = (float)d1 / d2;

                // printf("The second: %f,%d,%d\n",fac,d1,d2);

                if (fac < 0.95 || fac > 1.05) // 不合格
                {
                    // printf("The second is fail!\n");
                    cnt = 0;
                    DrawTouchPoint(display->lcddev.width - 20, display->lcddev.height - 20, WHITE); // 清除点4
                    DrawTouchPoint(20, 20, RED);                                                    // 画点1
                    ShowAdjInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                                pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); // 显示数据
                    continue;
                } // 正确了

                // 对角线相等
                tem1 = abs(pos_temp[1][0] - pos_temp[2][0]); // x1-x3
                tem2 = abs(pos_temp[1][1] - pos_temp[2][1]); // y1-y3
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = std::sqrt(tem1 + tem2); // 得到1,4的距离

                tem1 = abs(pos_temp[0][0] - pos_temp[3][0]); // x2-x4
                tem2 = abs(pos_temp[0][1] - pos_temp[3][1]); // y2-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = std::sqrt(tem1 + tem2); // 得到2,3的距离
                fac = (float)d1 / d2;
                if (fac < 0.95 || fac > 1.05) // 不合格
                {
                    cnt = 0;
                    DrawTouchPoint(display->lcddev.width - 20, display->lcddev.height - 20, WHITE); // 清除点4
                    DrawTouchPoint(20, 20, RED);                                                    // 画点1
                    ShowAdjInfo(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                                pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); // 显示数据
                    continue;
                } // 正确了
                // 计算结果
                xfac = (float)(display->lcddev.width - 40) / (pos_temp[1][0] - pos_temp[0][0]); // 得到xfac
                xoff = (display->lcddev.width - xfac * (pos_temp[1][0] + pos_temp[0][0])) / 2;  // 得到xoff

                yfac = (float)(display->lcddev.height - 40) / (pos_temp[2][1] - pos_temp[0][1]); // 得到yfac
                yoff = (display->lcddev.height - yfac * (pos_temp[2][1] + pos_temp[0][1])) / 2;  // 得到yoff
                if (fabsf(xfac) > 2 || fabsf(yfac) > 2)                                          // 触屏和预设的相反了.
                {
                    cnt = 0;
                    DrawTouchPoint(display->lcddev.width - 20, display->lcddev.height - 20, WHITE); // 清除点4
                    DrawTouchPoint(20, 20, RED);                                                    // 画点1
                    gui->ShowString(40, 26, 16, (u8 *)"TP Need readjust!", 1);
                    touchtype = !touchtype; // 修改触屏类型.
                    if (touchtype)          // X,Y方向与屏幕相反
                    {
                        CMD_RDX = 0X90;
                        CMD_RDY = 0XD0;
                    } else // X,Y方向与屏幕相同
                    {
                        CMD_RDX = 0XD0;
                        CMD_RDY = 0X90;
                    }
                    continue;
                }
                display->POINT_COLOR = BLUE;
                display->Clear(WHITE);                                            // 清屏
                gui->ShowString(35, 110, 16, (u8 *)"Touch Screen Adjust OK!", 1); // 校正完成
                // DelayMs(1000);
                uint32_t nCount = 100000;
                for (; nCount != 0; nCount--) {
                }
                SaveAdjData();
                display->Clear(WHITE); // 清屏
                return;       // 校正完成
            }
        }
        // DelayMs(10);
        uint32_t nCount = 1000;
        for (; nCount != 0; nCount--) {
        }
        outtime++;
        if (outtime > 1000) {
            GetAdjData();
            break;
        }
    }
}

// 保存校准参数
void TouchScreen::SaveAdjData() {
    s32 temp;
    u8 flag;
    // 保存校正结果!
    temp = xfac * 100000000; // 保存x校正因素
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE, temp, 4);
    // ee_WriteBytes((u8 *)&temp,SAVE_ADDR_BASE,4);
    temp = yfac * 100000000; // 保存y校正因素
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE + 4, temp, 4);
    // ee_WriteBytes((uint8_t *)&temp,SAVE_ADDR_BASE+4,4);
    // 保存x偏移量
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE + 8, xoff, 2);
    // ee_WriteBytes((uint8_t *)&xoff,SAVE_ADDR_BASE+8,2);
    // 保存y偏移量
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE + 10, yoff, 2);
    // ee_WriteBytes((uint8_t *)&yoff,SAVE_ADDR_BASE+10,2);
    // 保存触屏类型
    // AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,touchtype);
    ee_WriteBytes(&touchtype, SAVE_ADDR_BASE + 12, 1);
    flag = 0X0A; // 标记校准过了
    // AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp);
    ee_WriteBytes(&flag, SAVE_ADDR_BASE + 13, 1);
}

/**
 * @brief 读取校准参数
 * @return u8 读取结果，0表示成功，1表示失败
 */
u8 TouchScreen::GetAdjData() {
    s32 tempfac;
    u8 flag;
    // tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);//读取标记字,看是否校准过！
    ee_ReadBytes(&flag, SAVE_ADDR_BASE + 13, 1);
    if (flag == 0X0A) // 触摸屏已经校准过了
    {
        tempfac = AT24CXX_ReadLenByte(SAVE_ADDR_BASE, 4);
        // ee_ReadBytes((uint8_t *)&tempfac, SAVE_ADDR_BASE, 4);
        xfac = (float)tempfac / 100000000; // 得到x校准参数
        tempfac = AT24CXX_ReadLenByte(SAVE_ADDR_BASE + 4, 4);
        // ee_ReadBytes((uint8_t *)&tempfac, SAVE_ADDR_BASE+4, 4);
        yfac = (float)tempfac / 100000000; // 得到y校准参数
        // 得到x偏移量
        xoff = AT24CXX_ReadLenByte(SAVE_ADDR_BASE + 8, 2);
        // ee_ReadBytes((uint8_t *)&xoff, SAVE_ADDR_BASE+8, 2);
        // 得到y偏移量
        yoff = AT24CXX_ReadLenByte(SAVE_ADDR_BASE + 10, 2);
        // ee_ReadBytes((uint8_t *)&yoff, SAVE_ADDR_BASE+10, 2);
        // touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//读取触屏类型标记
        ee_ReadBytes(&touchtype, SAVE_ADDR_BASE + 12, 1);
        if (touchtype) // X,Y方向与屏幕相反
        {
            CMD_RDX = 0X90;
            CMD_RDY = 0XD0;
        } else // X,Y方向与屏幕相同
        {
            CMD_RDX = 0XD0;
            CMD_RDY = 0X90;
        }
        return 1;
    }
    return 0;
}

// 读取触摸屏坐标
int TouchScreen::ReadTouch(uint16_t *x1, uint16_t *y1) {
    int rtn = 0;
    if (ReadPEN() == 0) // 有按键按下
    {
        if (ReadXY2(&x, &y)) // 读取屏幕坐标
        {
            x = xfac * x + xoff; // 将结果转换为屏幕坐标
            y = yfac * y + yoff;
        }
        *x1 = x;
        *y1 = y;
        rtn = 1;
    }
    return rtn;
}

/**
 * @brief 画一个触摸点
 * @param x, y 点的X和Y坐标
 * @param color 点的颜色
 */
void TouchScreen::DrawTouchPoint(u16 x, u16 y, u16 color) {
    display->POINT_COLOR = color;
    gui->DrawLine(x - 12, y, x + 13, y); // 横线
    gui->DrawLine(x, y - 12, x, y + 13); // 竖线
    display->DrawPoint(x + 1, y + 1);
    display->DrawPoint(x - 1, y + 1);
    display->DrawPoint(x + 1, y - 1);
    display->DrawPoint(x - 1, y - 1);
    gui->Circle(x, y, display->POINT_COLOR, 6, 0); // 画中心圈
}

/**
 * @brief 画一个大点
 * @param x, y 点的X和Y坐标
 * @param color 点的颜色
 */
void TouchScreen::DrawBigPoint(u16 x, u16 y, u16 color) {
    display->POINT_COLOR = color;
    display->DrawPoint(x, y); // 中心点
    display->DrawPoint(x + 1, y);
    display->DrawPoint(x, y + 1);
    display->DrawPoint(x + 1, y + 1);
}

/**
 * @brief 显示校准信息
 * @param x0, y0 第一个点的X和Y坐标
 * @param x1, y1 第二个点的X和Y坐标
 * @param x2, y2 第三个点的X和Y坐标
 * @param x3, y3 第四个点的X和Y坐标
 * @param fac 校准系数
 */
void TouchScreen::ShowAdjInfo(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac) {
    display->POINT_COLOR = RED;
    gui->ShowString(40, 160, 16, (u8 *)"x1:", 1);
    gui->ShowString(40 + 80, 160, 16, (u8 *)"y1:", 1);
    gui->ShowString(40, 180, 16, (u8 *)"x2:", 1);
    gui->ShowString(40 + 80, 180, 16, (u8 *)"y2:", 1);
    gui->ShowString(40, 200, 16, (u8 *)"x3:", 1);
    gui->ShowString(40 + 80, 200, 16, (u8 *)"y3:", 1);
    gui->ShowString(40, 220, 16, (u8 *)"x4:", 1);
    gui->ShowString(40 + 80, 220, 16, (u8 *)"y4:", 1);
    gui->ShowString(40, 240, 16, (u8 *)"fac is:", 1);
    gui->ShowNum(40 + 24, 160, x0, 4, 16);                    // 显示数值
    gui->ShowNum(40 + 24 + 80, 160, y0, 4, 16);               // 显示数值
    gui->ShowNum(40 + 24, 180, x1, 4, 16);                    // 显示数值
    gui->ShowNum(40 + 24 + 80, 180, y1, 4, 16);               // 显示数值
    gui->ShowNum(40 + 24, 200, x2, 4, 16);                    // 显示数值
    gui->ShowNum(40 + 24 + 80, 200, y2, 4, 16);               // 显示数值
    gui->ShowNum(40 + 24, 220, x3, 4, 16);                    // 显示数值
    gui->ShowNum(40 + 24 + 80, 220, y3, 4, 16);               // 显示数值
    gui->ShowNum(40 + 56, display->lcddev.width, fac, 3, 16); // 显示数值,该数值必须在95~105范围之内.
}

// 运行触摸屏测试
void TouchScreen::Test() {
    Scan(0);
    // oled_ShowNum(0,0,x,4,17);
    // oled_ShowNum(0,2,y,4,17);
    // oled_ShowNum(40,0,display->lcddev.width,4,17);
    // oled_ShowNum(40,2,display->lcddev.height,4,17);
    if (sta & TP_PRES_DOWN) // 触摸屏被按下
    {
        DrawBigPoint(x, y, display->POINT_COLOR);
    }
    // DelayMs(3);
}

/**
 * @brief 发送一个字节到触摸屏
 * @param num 要发送的字节
 */
void TouchScreen::WriteByte(u8 num) {
    // u8 count=0;
    // for(count=0;count<8;count++)
    // {
    // 	if(num&0x80)TDIN=1;
    // 	else TDIN=0;
    // 	num<<=1;
    // 	TCLK=0;
    // 	TCLK=1;		//上升沿有效
    // }
    bsp_spiWrite0(num);
}

/**
 * @brief 读取触摸屏的AD转换值
 * @param CMD 发送的命令字
 * @return u16 返回读取的AD转换值
 */
u16 TouchScreen::ReadAD(u8 CMD) {
    u8 count = 0;
    u16 Num = 0;
    SetTCS(0);      // 选中触摸屏IC
    WriteByte(CMD); // 发送命令字
    // DelayUs(6);//ADS7846的转换时间最长为6us
    uint32_t nCount = 60; // 延时60us
    for (; nCount != 0; nCount--) {
    }
    // TCLK=0;
    // delay_us(1);
    // TCLK=1;		//给1个时钟，清除BUSY
    // TCLK=0;
    // for(count=0;count<16;count++)//读出16位数据,只有高12位有效
    // {
    // 	Num<<=1;
    // 	TCLK=0;	//下降沿有效
    // 	TCLK=1;
    // 	if(DOUT)Num++;
    // }
    Num = (uint16_t)bsp_spiRead1();
    Num <<= 8;
    Num |= (uint16_t)bsp_spiRead1();
    Num >>= 4; // 只有高12位有效.
    SetTCS(1); // 释放片选
    return (Num);
}

/**
 * @brief 带滤波的坐标读取
 * @param xy 读取的坐标类型，CMD_RDX表示X坐标，CMD_RDY表示Y坐标
 * @return u16 返回读取的坐标值
 */
u16 TouchScreen::ReadXOY(u8 xy) {
    u16 i, j;
    u16 buf[READ_TIMES];
    u16 sum = 0;
    u16 temp;
    for (i = 0; i < READ_TIMES; i++)
        buf[i] = ReadAD(xy);
    for (i = 0; i < READ_TIMES - 1; i++) // 排序
    {
        for (j = i + 1; j < READ_TIMES; j++) {
            if (buf[i] > buf[j]) // 升序排列
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    sum = 0;
    for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++)
        sum += buf[i];
    temp = sum / (READ_TIMES - 2 * LOST_VAL);
    return temp;
}

/**
 * @brief 读取触摸屏坐标
 * @param x 输出X坐标
 * @param y 输出Y坐标
 * @return u8 返回1表示成功，0表示失败
 */
u8 TouchScreen::ReadXY(u16 *x, u16 *y) {
    SetSPISpeed(SPI_BaudRatePrescaler_16); // SPI速度切换至5MHZ以下
    u16 xtemp, ytemp;
    xtemp = ReadXOY(CMD_RDX);
    ytemp = ReadXOY(CMD_RDY);
    // if(xtemp<100||ytemp<100)return 0;//读数失败
    *x = xtemp;
    *y = ytemp;
    SetSPISpeed(SPI_BaudRatePrescaler_2); // SPI速度切换回36MHZ
    return 1;                             // 读数成功
}

/**
 * @brief 双方向读取坐标，进行两次采样
 * @param x 输出X坐标
 * @param y 输出Y坐标
 * @return u8 返回1表示成功，0表示失败
 */
u8 TouchScreen::ReadXY2(u16 *x, u16 *y) {
    u16 x1, y1;
    u16 x2, y2;
    u8 flag;
    flag = ReadXY(&x1, &y1);
    if (flag == 0)
        return (0);
    flag = ReadXY(&x2, &y2);
    if (flag == 0)
        return (0);
    if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) // 前后两次采样在+-50内
        && ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE))) {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;
        return 1;
    } else
        return 0;
}

// 设置触摸片选信号
void TouchScreen::SetTCS(uint8_t bit) {
    GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)bit);
}

// 读取触摸中断信号
uint8_t TouchScreen::ReadPEN() {
    return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
}

// 设置SPI速度
void TouchScreen::SetSPISpeed(uint16_t SPI_BaudRatePrescaler) {
    bsp_SPI_Init(SPI_Direction_2Lines_FullDuplex | SPI_Mode_Master | SPI_DataSize_8b | SPI_CPOL_Low | SPI_CPHA_1Edge |
                 SPI_NSS_Soft | SPI_BaudRatePrescaler | SPI_FirstBit_MSB);
    SPI_HARD->I2SCFGR &= SPI_Mode_Select; /* 选择SPI模式，不是I2S模式 */
    SPI_HARD->CRCPR = 7;                  /* 一般不用 */
    SPI_Cmd(SPI_HARD, DISABLE);           /* 先禁止SPI  */
    SPI_Cmd(SPI_HARD, ENABLE);            /* 使能SPI  */
}

/**
 * @brief 向EEPROM写入数据
 * @param WriteAddr 写入地址
 * @param DataToWrite 要写入的数据
 * @param Len 写入长度
 */
void TouchScreen::WriteEEPROM(u16 WriteAddr, u32 DataToWrite, u8 Len) {
    u8 t, data;
    for (t = 0; t < Len; t++) {
        // AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
        data = DataToWrite >> (8 * t);
        ee_WriteBytes(&data, WriteAddr + t, 1);
    }
}

/**
 * @brief 从EEPROM读取数据
 * @param ReadAddr 读取地址
 * @param Len 读取长度
 * @return u32 读取的数据
 */
u32 TouchScreen::ReadEEPROM(u16 ReadAddr, u8 Len) {
    u8 t, data;
    u32 temp = 0;
    for (t = 0; t < Len; t++) {
        ee_ReadBytes(&data, ReadAddr + Len - t - 1, 1);
        temp <<= 8;
        temp += data;
    }
    return temp;
}

u16 TouchScreen::GetX0() const {
    return x0;
}
u16 TouchScreen::GetY0() const {
    return y0;
}
u16 TouchScreen::GetX() const {
    return x;
}
u16 TouchScreen::GetY() const {
    return y;
}
u8 TouchScreen::GetStatus() const {
    return sta;
}
