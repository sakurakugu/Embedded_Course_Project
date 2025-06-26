/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.c
*	版    本 : V1.1
*	说    明 : 这是硬件底层驱动程序模块的主文件。主要提供 bsp_Init()函数供主程序调用。主程序的每个c文件
*			可以在开头添加 #include "board.h" 来包含所有的外设驱动模块。
*
*
*********************************************************************************************************
*/

#include "bsp_tlink.h"

unsigned char e8266tomcu_str[1024];//用来保存wifi模块发过来的数据
unsigned char Txto8266Buf[1024] = "#11,22,1,1,30#";
unsigned int e8266tomcu_num=0;


unsigned char time1sflag = 0;
unsigned char timensflag = 0;
unsigned char linkokflag = 0;

unsigned char E8266inittotlink(void){
    static unsigned char E8266state = 0,timeout = 0;
		if(timeout>60){
	     E8266state = 0; //如果超时没有接收到心跳包则进入异常处理
			 linkokflag = 0; //连接失效
		   timeout = 0;
			 TLINK_OK_LED_OFF();//连接云平台指示灯
	  }	
		switch(E8266state){
	  case 0:
		timeout++;
	  if(timeout == 3)
			GPIO_ResetBits(ESP_EN_PORT,ESP_EN_GPIO_PIN);
		if(timeout == 4)
		{
			timeout = 0;
			GPIO_SetBits(ESP_EN_PORT,ESP_EN_GPIO_PIN);
		}
	  if(strstr((const char *)e8266tomcu_str,"ready")!=0){
	     Usart_SendString(DEBUG_USARTx,e8266tomcu_str);
			 memset(e8266tomcu_str,0,1024);
		   timeout = 0;
		   e8266tomcu_num = 0;
	     //mcuto8266_str("AT+CWMODE=1\r\n");
			 ESP_SendString(ESP_USARTx,(unsigned char*)"AT+CWMODE=1\r\n");
		   E8266state = 1;
	  }
	  break;
	  case 1:
	  timeout++;
	  if(strstr((const char *)e8266tomcu_str,"OK")!=0){
	     Usart_SendString(DEBUG_USARTx,e8266tomcu_str);
			 memset(e8266tomcu_str,0,1024);
		   timeout = 0;
		   e8266tomcu_num = 0;
			 ESP_SendString(ESP_USARTx,(unsigned char*)"AT+CWJAP=\"B702\",\"123456789\"\r\n");
		 //mcuto8266_str("AT+CWJAP=\"zsc\",\"13631103\"\r\n");
			 E8266state = 2;
	  }
	  break;
	  case 2:
	  timeout++;
	  if(strstr((const char *)e8266tomcu_str,"OK")!=0){
	     Usart_SendString(DEBUG_USARTx,e8266tomcu_str);
			 memset(e8266tomcu_str,0,1024);
		   timeout = 0;
		   e8266tomcu_num = 0;
			 ESP_SendString(ESP_USARTx,(unsigned char*)"AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647\r\n");
		   //mcuto8266_str("AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647\r\n");
		   E8266state = 3;
	  }
	  break;
	  case 3:
	  timeout++;
	  if(strstr((const char *)e8266tomcu_str,"OK")!=0){
	     Usart_SendString(DEBUG_USARTx,e8266tomcu_str);
			 memset(e8266tomcu_str,0,1024);
		   timeout = 0;
		   e8266tomcu_num = 0;
			 ESP_SendString(ESP_USARTx,(unsigned char*)"AT+CIPMODE=1\r\n");
		   //mcuto8266_str("AT+CIPMODE=1\r\n");
		   E8266state = 4;
	  }
	  break;
	  case 4:
	  timeout++;
	  if(strstr((const char *)e8266tomcu_str,"OK")!=0){
	     Usart_SendString(DEBUG_USARTx,e8266tomcu_str);
			 memset(e8266tomcu_str,0,1024);
		   timeout = 0;
		   e8266tomcu_num = 0;
			 ESP_SendString(ESP_USARTx,(unsigned char*)"AT+CIPSEND\r\n");
		   E8266state = 5;
	  }
	  break;
	  case 5:
	  timeout++;
	  if(strstr((const char *)e8266tomcu_str,">")!=0){
	     Usart_SendString(DEBUG_USARTx,e8266tomcu_str);
			 memset(e8266tomcu_str,0,1024);
		   timeout = 0;
		   e8266tomcu_num = 0;
			 ESP_SendString(ESP_USARTx,(unsigned char*)"U1M7X495G1571V38");
		 //mcuto8266_str("XXUN3WE5D1U74ARO");  //发送设备序列号
		   E8266state = 6;
		   linkokflag = 1;
		   timensflag = 0;//必须重新开始计时，否则程序不正常
	  }
	  break;
	  case 6:
	  timeout++;
	  if(strstr((const char *)e8266tomcu_str,"ok")!=0){
	     //接受到服务器心跳包就亮灯
			 TLINK_OK_LED_ON();
	     Usart_SendString(DEBUG_USARTx,e8266tomcu_str);
			 memset(e8266tomcu_str,0,1024);
		   timeout = 0;
		   e8266tomcu_num = 0;
	  }
	  break;
  }
	return 1;
}




