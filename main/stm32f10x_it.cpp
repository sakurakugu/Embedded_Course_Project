/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_spi_bus.h"
#include "bsp_usart.h"
#include "bsp_ESPxx.h"
#include "bsp_exti.h"
#include "bsp_led.h"
#include "util.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
#if 0
	TimingDelay_Decrement();	
#elif	1
	static uint16_t ms_count = 0,s_count = 0,ns_count = 0;
	MsCount--;
	bsp_RunPer1ms = 1;		/* 每隔1ms调用一次此函数，此函数在 bsp.c */
	
	if (++ms_count > 10)
	{
		ms_count = 0;
		bsp_RunPer10ms = 1;	/* 每隔10ms调用一次此函数，此函数在 bsp.c */
	}
	if(++s_count > 1000)
	{
			s_count = 0;
			bsp_RunPer1s = 1;
			if(++ns_count > 0){
				ns_count = 0;
				timensflag = 1;
			}
	}
#endif
}

void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		// LED1 取反		
		LED1_TOGGLE;
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	} 
#if 0
 //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		// LED2 取反		
		LED2_TOGGLE;
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  	
#endif
}

void KEY2_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		// LED2 取反		
		LED2_TOGGLE;
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}

uint8_t uartdebugBuf[50];
uint8_t Bufn = 0;

// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
#if 0
	uint8_t ucTemp,num;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		uartdebugBuf[Bufn] = USART_ReceiveData(DEBUG_USARTx);
		if(Bufn != 0){
			if((uartdebugBuf[Bufn-1] == 0x0D) && (uartdebugBuf[Bufn] == 0x0A)){
				Usart_SendString(DEBUG_USARTx,uartdebugBuf);
				JQ8900_SndStr(uartdebugBuf,Bufn+1);
				//ESP_SendString(ESP_USARTx,uartdebugBuf);
				ESP_SendCommand(uartdebugBuf,Bufn+1);
				memset(uartdebugBuf,0,50);
				
				Bufn = 0;
				return;
			}
		}
		if(++Bufn >=50) Bufn = 0;
	}	
#elif 1
	uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
    Usart_SendByte(DEBUG_USARTx,ucTemp); 
		Usart_SendByte(ESP_USARTx,ucTemp);
	}	
#endif
}


// ESP串口中断服务函数
void ESP_USART_IRQHandler(void)
{
#if 0
	uint8_t ucTemp;
	if(USART_GetITStatus(ESP_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(ESP_USARTx);
    Usart_SendByte(DEBUG_USARTx,ucTemp);    
	}	
		//test(ucTemp);
#elif 1
	 static unsigned char wifitomcudatastate = 0;
   unsigned char data;
	 if(USART_GetITStatus(ESP_USARTx,USART_IT_RXNE)!=RESET)
	 {		
			data = USART_ReceiveData(ESP_USARTx);  	
			if(linkokflag==1)
			{
				switch(wifitomcudatastate)
				{
					case 0:   
					if(data=='[')
					{//接收8266发过来的数据,判断数据是否为[，是则正式接收
						wifitomcudatastate = 1;
						e8266tomcu_num = 0;
					}
					break;	
					case 1:	
					e8266tomcu_str[e8266tomcu_num]=data;
					e8266tomcu_num++;
					if(e8266tomcu_str[e8266tomcu_num-1]==']')
					{//接收完服务器发过来的数据
						e8266tomcu_str[e8266tomcu_num-1]='\0';
						Usart_SendString(DEBUG_USARTx,e8266tomcu_str);
						
						if(strstr((const char *)e8266tomcu_str,"ok") != 0)
						{
							wifitomcudatastate = 0;
							break;
						}
						//////判断开关代码在此添加////////////////////////////
						
						memset(e8266tomcu_str,0,e8266tomcu_num);
						e8266tomcu_num=0;
						wifitomcudatastate = 0;
					}
					break;
				}
			}
			else
			{
				if(data != 0)
				{
					e8266tomcu_str[e8266tomcu_num]=data;
					e8266tomcu_num++;
				}
			}
		}
		if(e8266tomcu_num>=1000)
			e8266tomcu_num=0;
#endif
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
