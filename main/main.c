/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试按键
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "bsp.h"

unsigned char JQ8900Buff[] = {0xAA,0X07,0X02,0X00,0X00,0X00};


void SetRGBColor(uint32_t rgb) 
{ 
     //根据颜色值修改定时器的比较寄存器值 
    TIM_SetCompare1(TIM8,(uint8_t)rgb);
    TIM_SetCompare2(TIM8,(uint8_t)(rgb>>16));
    TIM_SetCompare3(TIM8,(uint8_t)(rgb>>8));   
    //COLOR_TIMx->COLOR_RED_CCRx = (uint8_t)(rgb>>16);      //R 
    //COLOR_TIMx->COLOR_GREEN_CCRx = (uint8_t)(rgb>>8);   //G 
    //COLOR_TIMx->COLOR_BLUE_CCRx = (uint8_t)rgb;           //B
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	
	uint32_t time = 0,Key_time = 0,ESPReset = 0;
	uint8_t KeyNum = 0,MotorSpeed = 170;
	float temprature = 0,Rt,Temp;
	uint16_t adcValue;
  bsp_Init();
	/* 发送一个字符串 */
	printf("OLED测试例程\r\n");		//lm75A软件模拟i2c测试例程
	oled_ShowCHStr(0,0,(u8 *)"我们");
	oled_ShowNum(30,0,3251546,7,16);//不支持自动换行
	oled_ShowString(0,2,(u8 *)"hello");//不够自动换行
	oled_ShowChar(70,2,(u8)'A');//页地址模式，x坐标回0
	oled_ShowCHinese(90,2,1);//显示第1个汉字 
	oled_DrawBMP(20,4,64,32,(u8 *)BMP1);
	//oledWriteByte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
	//oledWriteByte(0xA7, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
	
	//水平滚动
	// oledWriteByte(0x2E, OLED_CMD);
	// oledWriteByte(0x29, OLED_CMD);
	// oledWriteByte(0x00, OLED_CMD);
	// oledWriteByte(0x04, OLED_CMD);
	// oledWriteByte(0x00, OLED_CMD);
	// oledWriteByte(0x07, OLED_CMD);
	// oledWriteByte(0x00, OLED_CMD);
	// oledWriteByte(0x2F, OLED_CMD);
	// GUI_DrawFont16(0,0,RED,WHITE,(u8 *)"深",0);
	// LCD_Clear(GBLUE);
	// LCD_ShowChar(80,80,RED,0X0000,'h',16,1);//0带背景，1和背景颜色一样
	// LCD_ShowChar(50,80,RED,0X0000,'h',16,0);//0带背景，1和背景颜色一样
	// LCD_ShowString(120,90,16,(u8 *)"ABC",1);
	// LCD_ShowNum(230,460,234567,6,16);//x:0-320   y:0-480
	// GUI_DrawFont16(10,50,RED,GBLUE,(u8 *)"深",1);
	// Show_Str(10,50,RED,WHITE,(u8 *)"深圳市全动电子zsc",16,0);
	// gui_circle(40,400,RED,10,1);
	// Gui_Drawbmp16(20,200,242,242,gImage_zsc);
	
	SetRGBColor(0x0000FF);

	while(1)                            
	{	   
		
		if(bsp_RunPer1ms == 1){
			bsp_RunPer1ms = 0;
			segFlash();
			dzFlash();
			TP_test();
		}
		
		if(bsp_RunPer10ms == 1){
			bsp_RunPer10ms = 0;
			KeyNum = read_key();
			
			if(KeyNum == 1){
				LED3_TOGGLE;
				JQ8900Buff[0] = 1;
				JQ8900Buff[1] = 2;	   
				JQ8x00_ZuHeBoFang(JQ8900Buff, 2);
				//printf("按键 %d 按下\r\n",KeyNum);
				if(MotorSpeed >=250)	MotorSpeed = 170;
				MotorSpeed = MotorSpeed + 10;
				TIM_SetCompare2(TIM4,MotorSpeed);  
				TIM_SetCompare3(TIM4,MotorSpeed);
			}
			if(KeyNum == 2){
				LED2_TOGGLE;
				JQ8x00_AppointPlay(JQ8X00_FLASH,"01");
				DCMotor_Dir_Config(1);
			}
			if(KeyNum == 3){
				LED2_TOGGLE;
				//JQ8x00_Command_Data(AppointTrack,2);
				temprature = LM75_readTemp();
				//temprature = -10.6;
				printf("当前温度是 %.3f 度\r\n",temprature);	
				DCMotor_Dir_Config(0);
			}
			if(KeyNum == 4){
				LED2_TOGGLE;
				//JQ8x00_Command_Data(SetVolume, 30);
				ee_Test();//EEPROM测试
			}	
		}
		if(bsp_RunPer1s == 1)
		{
			bsp_RunPer1s = 0;
			
			adcValue = getAdc(RV_ADC_CHANNEL);
			printf("RV_ADC值是 %d\r\n",adcValue);	
			adcValue = getAdc(NTC_ADC_CHANNEL);
			
			printf("NTC_ADC值是 %d\r\n",adcValue);
			Rt = adcValue*10000UL/(4095-adcValue);			
			Temp = 1/(log(Rt/10000) /3950 + 1/(25+273.15))-273.15;
			printf("NTC_温度是 %.3f\r\n",Temp);	
			
			E8266inittotlink();
			//printf("1S\r\n");
			if((timensflag == 1)&&(linkokflag == 1))
			{
				//Txto8266Buf[1] = ;
				
				ESP_SendString(ESP_USARTx,(char *)Txto8266Buf);
	      timensflag = 0;
			}  
		}
	}
}
/*********************************************END OF FILE**********************/
