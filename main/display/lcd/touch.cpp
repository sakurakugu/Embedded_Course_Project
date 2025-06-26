//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//    SDI(MOSI)      接          PB15         //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PB14         //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PB9          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB13         //液晶屏SPI总线时钟信号
//      DC/RS        接          PB10         //液晶屏数据/命令控制信号
//       RST         接          PB12         //液晶屏复位控制信号
//       CS          接          PB11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PC10         //触摸屏触摸中断信号
//      T_DO         接          PC2          //触摸屏SPI总线读信号
//      T_DIN        接          PC3          //触摸屏SPI总线写信号
//      T_CS         接          PC13         //触摸屏片选控制信号
//      T_CLK        接          PC0          //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	

#include "touch.h"
    

#define SPI_Mode_Select      ((uint16_t)0xF7FF)
#define SPI_HARD	SPI1

_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0,
 	0,
	0,
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//默认为touchtype=0的数据.
uint8_t CMD_RDX=0XD0;
uint8_t CMD_RDY=0X90;

void TCS(uint8_t bit)
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)bit);
}

uint8_t PEN(void)
{
	return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
}

void TPSPI_SpeedToggle(uint16_t SPI_BaudRatePrescaler)
{
	bsp_SPI_Init(SPI_Direction_2Lines_FullDuplex | SPI_Mode_Master | SPI_DataSize_8b
	| SPI_CPOL_Low | SPI_CPHA_1Edge | SPI_NSS_Soft | SPI_BaudRatePrescaler | SPI_FirstBit_MSB);	
	SPI_HARD->I2SCFGR &= SPI_Mode_Select;		/* 选择SPI模式，不是I2S模式 */
	SPI_HARD->CRCPR = 7;		/* 一般不用 */
	SPI_Cmd(SPI_HARD, DISABLE);			/* 先禁止SPI  */
	SPI_Cmd(SPI_HARD, ENABLE);			/* 使能SPI  */
}
/*****************************************************************************
 * @name       :void TP_Write_Byte(uint8_t num)   
 * @date       :2018-08-09 
 * @function   :Write a byte data to the touch screen IC with SPI bus
 * @parameters :num:Data to be written
 * @retvalue   :None
******************************************************************************/  	 			    					   
void TP_Write_Byte(uint8_t num)    
{  
	// uint8_t count=0;   
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

/*****************************************************************************
 * @name       :uint16_t TP_Read_AD(uint8_t CMD)	  
 * @date       :2018-08-09 
 * @function   :Reading adc values from touch screen IC with SPI bus
 * @parameters :CMD:Read command,0xD0 for x,0x90 for y
 * @retvalue   :Read data
******************************************************************************/    
uint16_t TP_Read_AD(uint8_t CMD)	  
{ 	 
	// uint8_t count=0; 	  
	uint16_t Num=0; 
	TCS(0); 		//选中触摸屏IC
	TP_Write_Byte(CMD);//发送命令字
	DelayUs(6);//ADS7846的转换时间最长为6us
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
	Num<<=8;
	Num |= (uint16_t)bsp_spiRead1();
	Num>>=4;   	//只有高12位有效.
	TCS(1);		//释放片选	 
	return(Num);  
//#endif
}

#define READ_TIMES 5 	//读取次数
#define LOST_VAL 1	  	//丢弃值
/*****************************************************************************
 * @name       :uint16_t TP_Read_XOY(uint8_t xy)  
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates (x or y),
								Read the READ_TIMES secondary data in succession 
								and sort the data in ascending order,
								Then remove the lowest and highest number of LOST_VAL 
								and take the average
 * @parameters :xy:Read command(CMD_RDX/CMD_RDY)
 * @retvalue   :Read data
******************************************************************************/  
uint16_t TP_Read_XOY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

/*****************************************************************************
 * @name       :uint8_t TP_Read_XY(uint16_t *x,uint16_t *y)
 * @date       :2018-08-09 
 * @function   :Read touch screen x and y coordinates,
								The minimum value can not be less than 100
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y)
{
	TPSPI_SpeedToggle(SPI_BaudRatePrescaler_16);//SPI速度切换至5MHZ以下
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	TPSPI_SpeedToggle(SPI_BaudRatePrescaler_2);//SPI速度切换回36MHZ
	return 1;//读数成功
}

#define ERR_RANGE 50 //误差范围 
/*****************************************************************************
 * @name       :uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y) 
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates twice in a row, 
								and the deviation of these two times can not exceed ERR_RANGE, 
								satisfy the condition, then think the reading is correct, 
								otherwise the reading is wrong.
								This function can greatly improve the accuracy.
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 

/*****************************************************************************
 * @name       :void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color)
 * @date       :2018-08-09 
 * @function   :Draw a touch point,Used to calibrate							
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
								color:the color value of the touch point
 * @retvalue   :None
******************************************************************************/  
void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);//横线
	LCD_DrawLine(x,y-12,x,y+13);//竖线
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	gui_circle(x,y,POINT_COLOR,6,0);//画中心圈
}	

/*****************************************************************************
 * @name       :void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
 * @date       :2018-08-09 
 * @function   :Draw a big point(2*2)					
 * @parameters :x:Read x coordinate of the point
								y:Read y coordinate of the point
								color:the color value of the point
 * @retvalue   :None
******************************************************************************/   
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);//中心点 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}	

/*****************************************************************************
 * @name       :uint8_t TP_Scan(uint8_t tp)
 * @date       :2018-08-09 
 * @function   :Scanning touch event				
 * @parameters :tp:0-screen coordinate 
									 1-Physical coordinates(For special occasions such as calibration)
 * @retvalue   :Current touch screen status,
								0-no touch
								1-touch
******************************************************************************/  					  
uint8_t TP_Scan(uint8_t tp)
{			   
	if(PEN()==0)//有按键按下
	{
		if(tp)TP_Read_XY2(&tp_dev.x,&tp_dev.y);//读取物理坐标
		else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))//读取屏幕坐标
		{
	 		tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;//将结果转换为屏幕坐标
			tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//之前没有被按下
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//按键按下  
			tp_dev.x0=tp_dev.x;//记录第一次按下时的坐标
			tp_dev.y0=tp_dev.y;  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);//标记按键松开	
		}else//之前就没有被按下
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//返回当前的触屏状态
}

int  touch_read(uint16_t *x, uint16_t *y)
{
    int rtn = 0;
    if(PEN()==0)            //有按键按下
	{
        if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))//读取屏幕坐标
		{
	 		tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;//将结果转换为屏幕坐标
			tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
	 	} 
        *x = tp_dev.x;
        *y = tp_dev.y;
        rtn = 1;
	}   
	return rtn;
}
	  
//////////////////////////////////////////////////////////////////////////	 
//保存在EEPROM里面的地址区间基址,占用13个字节(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 255-13
/*****************************************************************************
 * @name       :void TP_Save_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Save calibration parameters		
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 										    
void TP_Save_Adjdata(void)
{
	s32 temp;			 
	uint8_t flag;
	//保存校正结果!		   							  
	temp=tp_dev.xfac*100000000;//保存x校正因素      
  AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);
	//ee_WriteBytes((uint8_t *)&temp,SAVE_ADDR_BASE,4);   
	temp=tp_dev.yfac*100000000;//保存y校正因素    
  AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//ee_WriteBytes((uint8_t *)&temp,SAVE_ADDR_BASE+4,4); 
	//保存x偏移量
  AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);
	//ee_WriteBytes((uint8_t *)&tp_dev.xoff,SAVE_ADDR_BASE+8,2); 		    
	//保存y偏移量
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);
	//ee_WriteBytes((uint8_t *)&tp_dev.yoff,SAVE_ADDR_BASE+10,2);	
	//保存触屏类型
	//AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);	
	ee_WriteBytes(&tp_dev.touchtype,SAVE_ADDR_BASE+12,1);
	flag=0X0A;//标记校准过了
	// AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp); 
	ee_WriteBytes(&flag,SAVE_ADDR_BASE+13,1);
}

// /*****************************************************************************
//  * @name       :uint8_t TP_Get_Adjdata(void)
//  * @date       :2018-08-09 
//  * @function   :Gets the calibration values stored in the EEPROM		
//  * @parameters :None
//  * @retvalue   :1-get the calibration values successfully
// 								0-get the calibration values unsuccessfully and Need to recalibrate
// ******************************************************************************/ 	
uint8_t TP_Get_Adjdata(void)
{					  
	s32 tempfac;
	uint8_t flag;
	// tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);//读取标记字,看是否校准过！
	ee_ReadBytes(&flag, SAVE_ADDR_BASE+13, 1); 		 
	if(flag==0X0A)//触摸屏已经校准过了			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);
		//ee_ReadBytes((uint8_t *)&tempfac, SAVE_ADDR_BASE, 4); 		   
		tp_dev.xfac=(float)tempfac/100000000;//得到x校准参数
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);	
		//ee_ReadBytes((uint8_t *)&tempfac, SAVE_ADDR_BASE+4, 4); 		          
		tp_dev.yfac=(float)tempfac/100000000;//得到y校准参数
	    //得到x偏移量
		tp_dev.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);
		//ee_ReadBytes((uint8_t *)&tp_dev.xoff, SAVE_ADDR_BASE+8, 2); 			   	  
 	    //得到y偏移量
		tp_dev.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);
		//ee_ReadBytes((uint8_t *)&tp_dev.yoff, SAVE_ADDR_BASE+10, 2); 				 	  
 		//tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//读取触屏类型标记
		ee_ReadBytes(&tp_dev.touchtype, SAVE_ADDR_BASE+12, 1);
		if(tp_dev.touchtype)//X,Y方向与屏幕相反
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	 
		}else				   //X,Y方向与屏幕相同
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;	 
		}		 
		return 1;	 
	}
	return 0;
}	
 
// //提示字符串
// const uint8_t* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

// /*****************************************************************************
//  * @name       :void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac)
//  * @date       :2018-08-09 
//  * @function   :Display calibration results	
//  * @parameters :x0:the x coordinates of first calibration point
// 								y0:the y coordinates of first calibration point
// 								x1:the x coordinates of second calibration point
// 								y1:the y coordinates of second calibration point
// 								x2:the x coordinates of third calibration point
// 								y2:the y coordinates of third calibration point
// 								x3:the x coordinates of fourth calibration point
// 								y3:the y coordinates of fourth calibration point
// 								fac:calibration factor 
//  * @retvalue   :None
// ******************************************************************************/ 	 					  
void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac)
{	  
	POINT_COLOR=RED;
	LCD_ShowString(40,160,16,(uint8_t *)"x1:",1);
 	LCD_ShowString(40+80,160,16,(uint8_t *)"y1:",1);
 	LCD_ShowString(40,180,16,(uint8_t *)"x2:",1);
 	LCD_ShowString(40+80,180, 16,(uint8_t *)"y2:",1);
	LCD_ShowString(40,200, 16,(uint8_t *)"x3:",1);
 	LCD_ShowString(40+80,200, 16,(uint8_t *)"y3:",1);
	LCD_ShowString(40,220, 16,(uint8_t *)"x4:",1);
 	LCD_ShowString(40+80,220, 16,(uint8_t *)"y4:",1);  
 	LCD_ShowString(40,240, 16,(uint8_t *)"fac is:",1);     
	LCD_ShowNum(40+24,160,x0,4,16);		//显示数值
	LCD_ShowNum(40+24+80,160,y0,4,16);	//显示数值
	LCD_ShowNum(40+24,180,x1,4,16);		//显示数值
	LCD_ShowNum(40+24+80,180,y1,4,16);	//显示数值
	LCD_ShowNum(40+24,200,x2,4,16);		//显示数值
	LCD_ShowNum(40+24+80,200,y2,4,16);	//显示数值
	LCD_ShowNum(40+24,220,x3,4,16);		//显示数值
	LCD_ShowNum(40+24+80,220,y3,4,16);	//显示数值
 	LCD_ShowNum(40+56,lcddev.width,fac,3,16); 	//显示数值,该数值必须在95~105范围之内.
}

// /*****************************************************************************
//  * @name       :uint8_t TP_Get_Adjdata(void)
//  * @date       :2018-08-09 
//  * @function   :Calibration touch screen and Get 4 calibration parameters
//  * @parameters :None
//  * @retvalue   :None
// ******************************************************************************/ 		 
void TP_Adjust(void)
{								 
	uint16_t pos_temp[4][2];//坐标缓存值
	uint8_t  cnt=0;	
	uint16_t d1,d2;
	uint32_t tem1,tem2;
	float fac; 	
	uint16_t outtime=0;
 	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//清屏   
	POINT_COLOR=RED;//红色 
	LCD_Clear(WHITE);//清屏 	   
	POINT_COLOR=BLACK;
	LCD_ShowString(10,40,16,(uint8_t *)"Please use the stylus click the",1);//显示提示信息
	LCD_ShowString(10,56,16,(uint8_t *)"cross on the screen.The cross will",1);//显示提示信息
	LCD_ShowString(10,72,16,(uint8_t *)"always move until the screen ",1);//显示提示信息
	LCD_ShowString(10,88,16,(uint8_t *)"adjustment is completed.",1);//显示提示信息

	TP_Drow_Touch_Point(20,20,RED);//画点1 
	tp_dev.sta=0;//消除触发信号 
	tp_dev.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误	 
	while(1)//如果连续10秒钟没有按下,则自动退出
	{
		tp_dev.scan(1);//扫描物理坐标
		//printf("The first: %f,%d,%d\n",fac,d1,d2);
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//按键按下了一次(此时按键松开了.)
		{	
			outtime=0;			
			tp_dev.sta&=~(1<<6);//标记按键已经被处理过了.
						   			   
			pos_temp[cnt][0]=tp_dev.x;
			pos_temp[cnt][1]=tp_dev.y;
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,WHITE);				//清除点1 
					TP_Drow_Touch_Point(lcddev.width-20,20,RED);	//画点2
					break;
				case 2:
 					TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);	//清除点2
					TP_Drow_Touch_Point(20,lcddev.height-20,RED);	//画点3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);			//清除点3
 					TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);	//画点4
					break;
				case 4:	 //全部四个点已经得到
	    		    //对边相等
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,2的距离
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到3,4的距离
					fac=(float)d1/d2;

					printf("The first: %f,%d,%d\n",fac,d1,d2);

					if(fac<0.95||fac>1.05||d1==0||d2==0)//不合格
					{
						printf("The first is fail!\n");
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//清除点4
   	 					TP_Drow_Touch_Point(20,20,RED);								//画点1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,3的距离
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,4的距离
					fac=(float)d1/d2;

					printf("The second: %f,%d,%d\n",fac,d1,d2);

					if(fac<0.95||fac>1.05)//不合格
					{
						printf("The second is fail!\n");
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//清除点4
   	 					TP_Drow_Touch_Point(20,20,RED);								//画点1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
						continue;
					}//正确了
								   
					//对角线相等
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,4的距离
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,3的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//清除点4
   	 					TP_Drow_Touch_Point(20,20,RED);								//画点1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
						continue;
					}//正确了
					//计算结果
					tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
					tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
						  
					tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
					tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff  
					if(fabsf(tp_dev.xfac)>2||fabsf(tp_dev.yfac)>2)//触屏和预设的相反了.
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//清除点4
   	 					TP_Drow_Touch_Point(20,20,RED);								//画点1
						LCD_ShowString(40,26, 16,(uint8_t *)"TP Need readjust!",1);
						tp_dev.touchtype=!tp_dev.touchtype;//修改触屏类型.
						if(tp_dev.touchtype)//X,Y方向与屏幕相反
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y方向与屏幕相同
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}			    
						continue;
					}		
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);//清屏
					LCD_ShowString(35,110, 16,(uint8_t *)"Touch Screen Adjust OK!",1);//校正完成
					DelayMs(1000);
					TP_Save_Adjdata();  
 					LCD_Clear(WHITE);//清屏   
					return;//校正完成				 
			}
		}
		DelayMs(10);
		outtime++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();
			break;
	 	} 
 	}
}		


/*****************************************************************************
 * @name       :uint8_t TP_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization touch screen
 * @parameters :None
 * @retvalue   :0-no calibration
								1-Has been calibrated
******************************************************************************/  
uint8_t TP_Init(void)
{			    		   
	//注意,时钟使能之后,对GPIO的操作才有效
	//所以上拉之前,必须使能时钟.才能实现真正的上拉输出
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO
		    		   
	//注意,时钟使能之后,对GPIO的操作才有效
	//所以上拉之前,必须使能时钟.才能实现真正的上拉输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_GPIOE, ENABLE);
	
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_13;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出 
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(GPIOC, &GPIO_InitStructure);	

	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //上拉输入
	// GPIO_Init(GPIOC, &GPIO_InitStructure);
 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);

  TP_Read_XY(&tp_dev.x,&tp_dev.y);//第一次读取初始化	 
 	// AT24CXX_Init();//初始化24CXX
	LCD_Clear(WHITE);
	if(TP_Get_Adjdata())return 0;//已经校准
	else			   //未校准?
	{ 										    
		LCD_Clear(WHITE);//清屏
	    TP_Adjust();  //屏幕校准 
		TP_Save_Adjdata();	 
	}		
	DelayMs(1000);
	TP_Get_Adjdata();	
	return 1; 									 
}

void TP_test(void)
{
	
	TP_Scan(0);
	// oled_ShowNum(0,0,tp_dev.x,4,17);
	// oled_ShowNum(0,2,tp_dev.y,4,17);
	// oled_ShowNum(40,0,lcddev.width,4,17);
	// oled_ShowNum(40,2,lcddev.height,4,17);
	if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
	{
		TP_Draw_Big_Point(tp_dev.x,tp_dev.y,POINT_COLOR);
    }
	//DelayMs(3);
}


/*****************************************************************************
 * @name       :void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
 * @date       :2018-08-09 
 * @function   :begins to write data with a length of Len 
								at the specified address in the AT24CXX
 * @parameters :WriteAddr:the address to start writing
								DataToWrite:the header address of the data array 
								Len:Length of data to be written(2 or 4)
 * @retvalue   :None
******************************************************************************/ 
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{  	
	uint8_t t,data;
	for(t=0;t<Len;t++)
	{
		//AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
		data = DataToWrite>>(8*t);
		ee_WriteBytes(&data,WriteAddr+t,1);
	}												    
}

/*****************************************************************************
 * @name       :uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
 * @date       :2018-08-09 
 * @function   :begins to read data with a length of Len 
								at the specified address in the AT24CXX,
								used to read 16bits or 32bits data
 * @parameters :ReadAddr:the address of start reading
								len:Length of data to be read(2 or 4)
 * @retvalue   :Read data
******************************************************************************/ 
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{  	
	uint8_t t,data;
	uint32_t temp=0;
	for(t=0;t<Len;t++)
	{
		ee_ReadBytes(&data,ReadAddr+Len-t-1,1);
		temp<<=8;
		temp+=data; 	 	
	}
	return temp;												    
}

