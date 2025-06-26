#pragma once
#include "stm32f10x.h"
#include "oledFont.h"
#include "bsp_spi_bus.h"
#include "config.h"

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0	//有DC线

#define OLED_DC_SET()  GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET)
#define OLED_DC_CLR()  GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET)

#define OLED_CS_SET()  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET)
#define OLED_CS_CLR()  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET)

#define OLED_RST_SET()  GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_SET)
#define OLED_RST_CLR()  GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_RESET)

//OLED模式设置
//0:4线串行模式
//1:并行8080模式   
  
void oledDisplayOn(void);
void oledDisplayOff(void);	   							   		    
void oled_Init(void);
void oled_Clear(void);
void oled_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void oled_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void oled_ShowChar(uint8_t x,uint8_t y,uint8_t ch);
void oledWriteByte(uint8_t dat, uint8_t cmd);
void oled_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
void oled_ShowAsciiString(uint8_t x,uint8_t y, uint8_t *p);	 
void oled_Set_Pos(unsigned char x, unsigned char y);
void oled_ShowChinese(unsigned char X , unsigned char Y, unsigned char *pstr);
void oled_ShowString(unsigned char X, unsigned char Y, unsigned char *str);
void oled_DrawBMP(unsigned char X,unsigned char Y,unsigned char Pix_x,unsigned char Pix_y,const unsigned char *BMP);

	 



