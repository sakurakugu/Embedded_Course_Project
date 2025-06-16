#include "touch.h"
#include "bsp_lcd.h" // 假设需要调用LCD函数进行显示

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
}

// 析构函数
TouchScreen::~TouchScreen() {
    // 清理资源如果有需要
}

// 初始化触摸屏
u8 TouchScreen::Init() {
    // 将原来的TP_Init函数内容移至此处
    // 这里简化为调用原有的C函数，实际实现时应将代码直接移植过来
    return TP_Init();
}

// 扫描触摸屏
u8 TouchScreen::Scan(u8 tp) {
    // 将原来的TP_Scan函数内容移至此处
    // 这里简化为调用原有的C函数，实际实现时应将代码直接移植过来
    u8 result = TP_Scan(tp);
    // 更新类的成员变量，假设原来的TP_Scan会更新全局变量tp_dev
    // 具体实现需要根据原代码调整
    // 这里仅作为示例
    sta = result;
    return result;
}

// 触摸屏校准
void TouchScreen::Adjust() {
    // 将原来的TP_Adjust函数内容移至此处
    TP_Adjust();
}

// 保存校准参数
void TouchScreen::SaveAdjData() {
    // 将原来的TP_Save_Adjdata函数内容移至此处
    TP_Save_Adjdata();
}

// 读取校准参数
u8 TouchScreen::GetAdjData() {
    // 将原来的TP_Get_Adjdata函数内容移至此处
    return TP_Get_Adjdata();
}

// 读取触摸屏坐标
int TouchScreen::ReadTouch(uint16_t *x, uint16_t *y) {
    // 将原来的touch_read函数内容移至此处
    return touch_read(x, y);
}

// 画一个坐标校准点
void TouchScreen::DrawTouchPoint(u16 x, u16 y, u16 color) {
    // 将原来的TP_Drow_Touch_Point函数内容移至此处
    TP_Drow_Touch_Point(x, y, color);
}

// 画一个大点
void TouchScreen::DrawBigPoint(u16 x, u16 y, u16 color) {
    // 将原来的TP_Draw_Big_Point函数内容移至此处
    TP_Draw_Big_Point(x, y, color);
}

// 显示校准信息
void TouchScreen::ShowAdjInfo(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac) {
    // 将原来的TP_Adj_Info_Show函数内容移至此处
    TP_Adj_Info_Show(x0, y0, x1, y1, x2, y2, x3, y3, fac);
}

// 运行触摸屏测试
void TouchScreen::Test() {
    // 将原来的TP_test函数内容移至此处
    TP_test();
}

// 向触摸屏控制芯片写入一个字节数据
void TouchScreen::WriteByte(u8 num) {
    // 将原来的TP_Write_Byte函数内容移至此处
    TP_Write_Byte(num);
}

// 读取AD转换值
u16 TouchScreen::ReadAD(u8 CMD) {
    // 将原来的TP_Read_AD函数内容移至此处
    return TP_Read_AD(CMD);
}

// 带滤波的坐标读取
u16 TouchScreen::ReadXOY(u8 xy) {
    // 将原来的TP_Read_XOY函数内容移至此处
    return TP_Read_XOY(xy);
}

// 双方向读取坐标
u8 TouchScreen::ReadXY(u16 *x, u16 *y) {
    // 将原来的TP_Read_XY函数内容移至此处
    return TP_Read_XY(x, y);
}

// 带加强滤波的双方向坐标读取
u8 TouchScreen::ReadXY2(u16 *x, u16 *y) {
    // 将原来的TP_Read_XY2函数内容移至此处
    return TP_Read_XY2(x, y);
}

// 设置触摸片选信号
void TouchScreen::SetTCS(uint8_t bit) {
    // 将原来的TCS函数内容移至此处
    TCS(bit);
}

// 读取触摸中断信号
uint8_t TouchScreen::ReadPEN() {
    // 将原来的PEN函数内容移至此处
    return PEN();
}

// 设置SPI速度
void TouchScreen::SetSPISpeed(uint16_t SPI_BaudRatePrescaler) {
    // 将原来的TPSPI_SpeedToggle函数内容移至此处
    TPSPI_SpeedToggle(SPI_BaudRatePrescaler);
}

/**
 * @brief 向EEPROM写入数据
 * @param WriteAddr 写入地址
 * @param DataToWrite 要写入的数据
 * @param Len 写入长度
 */
void TouchScreen::WriteEEPROM(u16 WriteAddr, u32 DataToWrite, u8 Len) {
    u8 t,data;
	for(t=0;t<Len;t++)
	{
		//AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
		data = DataToWrite>>(8*t);
		ee_WriteBytes(&data,WriteAddr+t,1);
	}	
}

/**
 * @brief 从EEPROM读取数据
 * @param ReadAddr 读取地址
 * @param Len 读取长度
 * @return u32 读取的数据
 */
u32 TouchScreen::ReadEEPROM(u16 ReadAddr, u8 Len) {
    u8 t,data;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		ee_ReadBytes(&data,ReadAddr+Len-t-1,1);
		temp<<=8;
		temp+=data; 	 	
	}
	return temp;	
}

// 以下是C兼容接口的实现，作为TouchScreen类的包装函数
#ifdef __cplusplus
extern "C" {
#endif
    // 定义一个全局变量tp_dev，与原C代码兼容
    _m_tp_dev tp_dev;

    u8 TP_Init(void) {
        // 调用类的方法
        return TouchScreen::GetInstance().Init();
    }

    u8 TP_Scan(u8 tp) {
        // 调用类的方法
        return TouchScreen::GetInstance().Scan(tp);
    }

    void TP_Adjust(void) {
        // 调用类的方法
        TouchScreen::GetInstance().Adjust();
    }

    void TP_Save_Adjdata(void) {
        // 调用类的方法
        TouchScreen::GetInstance().SaveAdjData();
    }

    u8 TP_Get_Adjdata(void) {
        // 调用类的方法
        return TouchScreen::GetInstance().GetAdjData();
    }

    int touch_read(uint16_t *x, uint16_t *y) {
        // 调用类的方法
        return TouchScreen::GetInstance().ReadTouch(x, y);
    }

    void TP_test(void) {
        // 调用类的方法
        TouchScreen::GetInstance().Test();
    }

    void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac) {
        // 调用类的方法
        TouchScreen::GetInstance().ShowAdjInfo(x0, y0, x1, y1, x2, y2, x3, y3, fac);
    }

#ifdef __cplusplus
}
#endif
