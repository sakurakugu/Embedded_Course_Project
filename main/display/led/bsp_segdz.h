#pragma once

#include "stm32f10x.h"
#include "bsp_spi_bus.h"
#include "config.h"

#define SEG595_Enable()  GPIO_ResetBits(SEG595_OE_PORT, SEG595_OE_PIN)
#define SEG595_DISable() GPIO_SetBits(SEG595_OE_PORT, SEG595_OE_PIN)
#define SEG_ST_CLK()	GPIO_ResetBits(SEG595_ST_PORT, SEG595_ST_PIN); GPIO_SetBits(SEG595_ST_PORT, SEG595_ST_PIN)

#define DZ595_Enable()  GPIO_ResetBits(DZ595_OE_PORT, DZ595_OE_PIN)
#define DZ595_DISable() GPIO_SetBits(DZ595_OE_PORT, DZ595_OE_PIN)
#define DZ_ST_CLK()	GPIO_ResetBits(DZ595_ST_PORT, DZ595_ST_PIN); GPIO_SetBits(DZ595_ST_PORT, DZ595_ST_PIN)

#define DISP_Enable() GPIO_ResetBits(DISP_EN_PORT,DISP_EN_PIN)




//定义SEG段在变量中7-0位		 
//   SIGA SIGB SIGC SIGD SIGE SIGF SIGG SIGDP
//0B  1    1    1    1    1    1    1    1    对应二进数位置
#define    SIGDP     0X01 //0B0000 0001
#define    SIGG      0X02 //0B0000 0010
#define    SIGF      0X04	//0B0000 0100
#define    SIGE      0X08	//0B0000 1000
#define    SIGD      0X10	//0B0001 0000
#define    SIGC      0X20	//0B0010 0000
#define    SIGB      0X40	//0B0100 0000
#define    SIGA      0X80	//0B1000 0000

/* 
  —A—
F|   	 |B
  —G—
E|     |C
  —D— .dp
*/
//数码管显示数字或字母的段码表
#define		SMG_0		SIGA|SIGB|SIGC|SIGD|SIGE|SIGF       	
#define		SMG_1		SIGB|SIGC                             	
#define		SMG_2		SIGA|SIGB|SIGD|SIGE|SIGG              	
#define		SMG_3		SIGA|SIGB|SIGC|SIGD|SIGG              	
#define		SMG_4		SIGB|SIGC|SIGF|SIGG                   	
#define		SMG_5		SIGA|SIGC|SIGD|SIGF|SIGG              	
#define		SMG_6		SIGA|SIGC|SIGD|SIGE|SIGF|SIGG         	
#define		SMG_7		SIGA|SIGB|SIGC                        	
#define		SMG_8		SIGA|SIGB|SIGC|SIGD|SIGE|SIGF|SIGG      	
#define		SMG_9		SIGA|SIGB|SIGC|SIGD|SIGF|SIGG         	
#define		SMG_A		SIGA|SIGB|SIGC|SIGE|SIGF|SIGG         	
#define		SMG_B		SIGC|SIGD|SIGE|SIGF|SIGG              	
#define		SMG_C		SIGG|SIGD|SIGE                  	
#define		SMG_D		SIGB|SIGC|SIGD|SIGE|SIGG              	
#define		SMG_E		SIGA|SIGD|SIGE|SIGF|SIGG              	
#define		SMG_F		SIGA|SIGE|SIGF|SIGG 


extern unsigned char Disp_data[];


void dzFlash(void);
void Led8Flash(uint8_t Left_R,uint8_t Left_L,uint8_t Right_R,uint8_t Right_L,uint8_t OffSet_Left_R,uint8_t OffSet_Left_L,uint8_t OffSet_Right_R,uint8_t OffSet_Right_L);
void segFlash(void);
void seg_dz_GPIOInit(void);

