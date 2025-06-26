#include "bsp_JQ8900.h"




 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void JQ8900_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	JQ8900_USART_GPIO_APBxClkCmd(JQ8900_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	JQ8900_USART_APBxClkCmd(JQ8900_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = JQ8900_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(JQ8900_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = JQ8900_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(JQ8900_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = JQ8900_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(JQ8900_USARTx, &USART_InitStructure);
	
	// 使能串口
	USART_Cmd(JQ8900_USARTx, ENABLE);		

  // 清除发送完成标志
	//USART_ClearFlag(USART1, USART_FLAG_TC);     
}

void JQ8900_SndStr(unsigned char * str,unsigned char num)
{
   unsigned char j;
    for(j=0;j<num;j++)
    {
       Usart_SendByte(JQ8900_USARTx,str[j]);
   }
}

/************************************************************************
  功能描述：起始码-指令类型-数据长度-校验和
  入口参数：   MODE：模式
  返 回 值： none
  其他说明： 不带数据的指令播放函数.无数数据传入
**************************************************************************/
void  JQ8x00_Command(UartCommand Command)
{
    uint8_t Buffer[4] = {0xaa};

    Buffer[1] = Command;                                    //指令类型
    Buffer[2] = 0x00;                                       //数据长度为0
    Buffer[3] = Buffer[0] +  Buffer[1] +  Buffer[2];        //校验和

    //if (JQ8X00_BusyCheck)
    //{
        //delay(10);
        //while (digitalRead(IO_JQ8X00_BusyCheck) == HIGH);   //等待语音模块取消“设备正忙”状态
    //}
    //Serial.write(Buffer, 4);
	JQ8900_SndStr(Buffer, 4);
}

/************************************************************************
  功能描述：起始码-指令类型-数据长度-数据-校验和
  入口参数：   *DAT：字符串指针,Len字符串长度
  返 回 值： none
  其他说明：
**************************************************************************/
void  JQ8x00_Command_Data(UartCommandData Command, uint8_t DATA)
{
    uint8_t Buffer[6] = {0xaa};
    uint8_t DataLen = 0;                                         //数据长度
    Buffer[1] = Command;                                    //指令类型
    if((Command != AppointTrack) && (Command != SetCycleCount) && (Command != SelectTrackNoPlay) && (Command != AppointTimeBack) && (Command != AppointTimeFast))        
                                                            //若待发送指令为只含一个数据的指令
    {
        Buffer[2] = 1;                                      //数据长度为1
        Buffer[3] = DATA;                                   //附加数据
        Buffer[4] = Buffer[0] +  Buffer[1] +  Buffer[2] + Buffer[3];
                                                            //校验和
        DataLen = 5;
    }
    else                                                    //若待发送指令为不止一个数据的指令
  {
        Buffer[2] = 2;                                      //数据长度为2
        Buffer[3] = DATA/256;                               //数据1
        Buffer[4] = DATA%256;                               //数据2
        Buffer[5] = Buffer[0] +  Buffer[1] +  Buffer[2] + Buffer[3] + Buffer[4];
                                                            //校验和
        DataLen = 6;
    }

    //if (JQ8X00_BusyCheck)
    //{
        //delay(10);
        //while (digitalRead(IO_JQ8X00_BusyCheck) == HIGH);   //等待语音模块取消“设备正忙”状态
   // }
    //Serial.write(Buffer, DataLen);                          //打包发送数据长度为DataLen的一组指令集
	JQ8900_SndStr(Buffer, DataLen);       
}

/************************************************************************
  功能描述：播放根目录指定文件名的音频文件
  入口参数：JQ8X00_Symbol:系统盘符，*DATA:需要播放的文件名
  返 回 值： none
  其他说明：播放FLASH根目录下文件名为00001.mp3的音频，JQ_8x00_AppointPlay(JQ8X00_FLASH,"00001");
**************************************************************************/
void JQ8x00_AppointPlay(JQ8X00_Symbol MODE, char *DATA)
{
    uint8_t Buffer[30] = {0xaa, 0x08};
    uint8_t i, j;
    Buffer[2] = 1 + strlen(DATA) + 5;                       //数据长度
    Buffer[3] = MODE;                                       //盘符
    Buffer[4] = '/';                                        //路径
    i = 5;
    while (*DATA)
    {
        Buffer[i++] =  *DATA;
        DATA++;
    }
    Buffer[i++] = '*';
    Buffer[i++] = '?';
    Buffer[i++] = '?';
    Buffer[i++] = '?';                                      //根据解析ANSI结果发送HEX数据
    for (j = 0; j < i; j++)
    {
        Buffer[i] = Buffer[i] + Buffer[j];                  //最后发送校验和
    }
    i++;

    //if (JQ8X00_BusyCheck)
    //{
        //delay(10);
        //while (digitalRead(IO_JQ8X00_BusyCheck) == HIGH);   //等待语音模块取消“设备正忙”状态
    //}

    //Serial.write(Buffer, i);                                //打包发送数据长度为i的一组指令集
	JQ8900_SndStr(Buffer, i);                      
}

/************************************************************************
  功能描述：组合播放函数
  入口参数：   *DAT：字符串指针,Len字符串长度
  返 回 值： none
  其他说明： 将需要播报的文件名放入数组中作为形参即可,例：组合播放01,02文件，则DATA内容为{1,2}，JQ8x00_ZuHeBoFang（DATA，2）
**************************************************************************/
void  JQ8x00_ZuHeBoFang(unsigned char *DATA, unsigned char Len)
{
    unsigned char CRC_data = 0, i = 3;
    unsigned char Buffer[30] = {0xaa, 0x1b};

    Buffer[2] = Len * 2;                                    //计算长度
    CRC_data = CRC_data + 0xaa + 0x1b + Buffer[2];
    while (Len--)
    {
        Buffer[i] = *DATA / 10 + 0x30;                      //取出十位
        CRC_data = CRC_data + Buffer[i];
        i++;
        Buffer[i] = *DATA % 10 + 0x30;                      //取出个位
        CRC_data = CRC_data + Buffer[i];
        i++;
        DATA++;
    }
    Buffer[i] = CRC_data;
    i++;
    //if (JQ8X00_BusyCheck)
    //{
        //delay(10);
        //while (digitalRead(IO_JQ8X00_BusyCheck) == HIGH);   //等待语音模块取消“设备正忙”状态
    //}

    //Serial.write(Buffer, i);                                //打包发送数据长度为i的一组指令集
	JQ8900_SndStr(Buffer, i);           
  }

/***************语音模块通信数据库已结束*************/
/*数据库已由“语音芯片模块技术交流群JQ8400/JQ8900/JR6001系列” @空白 推送更新并在2019年10月18日12:54:35应用*/
/*注释补充和预设指令表由李皇谛·李炜于2019年10月19日13:12:57完善*/
/********************应用例如下********************/
void JQ8900setup(void){                                               //Arduino的初始化程序，开机后只执行一次
  //if (JQ8X00_BusyCheck){
    //pinMode(IO_JQ8X00_BusyCheck, INPUT_PULLUP);             //忙检测引脚初始化
    //}
		JQ8900_Config();
    //Serial.begin(9600,SERIAL_8N1);                          //启动串口，通信频率为9600Hz，数据包长度为8，无校验位，占用1个停止位
    //delay(10);                                              //延时10ms，防止串口数据包被中断
    JQ8x00_Command_Data(SetVolume, 30);                     //设置音量为30
    //delay(10);                                              //延时10ms
   //JQ8x00_Command_Data(AppointTrack,2);                     //播放指定曲目2
   //JQ8x00_AppointPlay(JQ8X00_FLASH, "00002");               //播放FLASH下，文件名为00002的音频文件
   
}
