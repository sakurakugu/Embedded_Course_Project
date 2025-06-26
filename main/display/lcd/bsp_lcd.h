#pragma once

#include "stm32f10x.h"
#include "lcdFont.h"
#include "bsp_spi_bus.h"
#include "bsp_i2c_ee.h"
#include "bsp_SysTick.h"
#include "util.h"
#include "config.h"

//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				  //LCD ID
	uint8_t  dir;			  //横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	 wramcmd;		//开始写gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数 


//TFTLCD部分外要调用的函数		   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

#define LCD_DC_SET()  GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET)
#define LCD_DC_CLR()  GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET)

#define LCD_CS_SET()  GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET)
#define LCD_CS_CLR()  GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_RESET)

#define LCD_RST_SET()  GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_SET)
#define LCD_RST_CLR()  GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_RESET)

#define LCD_LED_ON()  GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET)
#define LCD_LED_OFF()  GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET)

				



#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 		0XFFE0
#define GBLUE		0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //
#define BRRED 			0XFC07 //
#define GRAY  			0X8430 //


#define DARKBLUE      	 0X01CF	//
#define LIGHTBLUE      	 0X7D7C	//
#define GRAYBLUE       	 0X5458 //

 
#define LIGHTGREEN     	0X841F //
#define LIGHTGRAY     	0XEF5B //
#define LGRAY 			0XC618 //

#define LGRAYBLUE      	0XA651 //
#define LBBLUE          0X2B12 //
	    															  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   
uint16_t LCD_RD_DATA(void);						    
void LCD_WR_DATA(uint8_t data);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
uint16_t LCD_ReadRAM(void);		   
uint16_t LCD_BGR2RGB(uint16_t c);
void LCD_SetParam(void);

void LCD_direction(uint8_t direction );


void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_Clear(uint16_t Color);	
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_WriteData_16Bit(uint16_t Data);

	 



