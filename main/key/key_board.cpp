#include "key_board.h"  

// 静态成员变量定义
uint8_t KeyBoard::key_state = 0;
uint8_t KeyBoard::timekey = 0;

// 构造函数
KeyBoard::KeyBoard() {
    Key_GPIO_Config();
    Key_Com_GPIO_Config();
}

// 析构函数
KeyBoard::~KeyBoard() {
    // 清理代码
}

/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void KeyBoard::Key_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK,ENABLE);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// 设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	//使用结构体初始化按键
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	//使用结构体初始化按键
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN; 
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	//使用结构体初始化按键
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);	
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN; 
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	//使用结构体初始化按键
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);	
	
}

void KeyBoard::Key_Com_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEYL_GPIO_CLK,ENABLE);
	
	//按键的公共端引脚输出
	GPIO_InitStructure.GPIO_Pin = KEYL1_GPIO_PIN|KEYL2_GPIO_PIN|KEYL3_GPIO_PIN; 
	// 设置按键的公共引脚为输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	//使用结构体初始化
	GPIO_Init(KEYL_GPIO_PORT, &GPIO_InitStructure);
	//输出0
	GPIO_ResetBits(KEYL_GPIO_PORT, KEYL1_GPIO_PIN|KEYL2_GPIO_PIN|KEYL3_GPIO_PIN);

}

 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
uint8_t KeyBoard::Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */

			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
			return 	KEY_ON;	 

	}
	else
		return KEY_OFF;
}

//状态机按键
#define key_input		GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN)
#define key_inputN	(KEY1_GPIO_PORT->IDR&(KEY1_GPIO_PIN|KEY2_GPIO_PIN|KEY3_GPIO_PIN|KEY4_GPIO_PIN))
#define key_input1	GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN)
#define key_input2	GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN)
#define key_input3	GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN)
#define key_input4	GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN)

#define ALLKEYOFF			(KEY1_GPIO_PIN|KEY2_GPIO_PIN|KEY3_GPIO_PIN|KEY4_GPIO_PIN) 
#define	KEY1		1
#define	KEY2		2
#define	KEY3		3
#define	KEY4		4

#define	KEY5		5
#define	KEY6		6
#define	KEY7		7
#define	KEY8		8

#define	KEY9		9
#define	KEY10		10
#define	KEY11		11
#define	KEY12		12

#define key_state_0	0               //定义状态
#define key_state_1	1
#define key_state_2	2
#define key_state_3	3

uint8_t KeyBoard::ReadKey()
{
	uint8_t key_return = 0;
#if		0
	switch (key_state)
	{
		case key_state_0:				// 按键初始态
			if (key_input == KEY_ON) 		// 读按键I/O电平
				key_state = key_state_1;	// 键被按下，状态转换到键确认态
			break;
		case key_state_1:				// 按键确认态
			if (key_input == KEY_ON)
			{
				key_return = 1;			// 按键仍按下，按键确认输出为"1" （1）
				key_state = key_state_2;	// 状态转换到键释放态
			}
			else
				key_state = key_state_0;	// 按键已抬起，转换到按键初始态
			break;
		case key_state_2:
			if (key_input == KEY_OFF){ //按键释放
			  key_state = key_state_0;	
			  //key_return = 1;
			}
			break;
	}	
#elif 0
		switch (key_state)
		{
			case key_state_0:				// 按键初始态
				if (key_input == KEY_ON) 		// 读按键I/O电平
					key_state = key_state_1;	// 键被按下，状态转换到键确认态
				break;
			case key_state_1:				// 按键确认态
				if (key_input == KEY_ON)
				{
					//key_return = 1;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
				else
					key_state = key_state_0;	// 按键已抬起，转换到按键初始态
				break;
			case key_state_2:
				if (key_input == KEY_ON){ 
					if(++timekey>100){ 
							timekey = 0;
							key_return = 2;
							key_state = key_state_3;
					 }
				}
				else{
					key_return = 1;
					timekey = 0;
					key_state = key_state_0;
				}
				break;
			case key_state_3:
				if (key_input == KEY_OFF){ //按键释放
					key_state = key_state_0;	
			  //key_return = 0;
				}
				break;
		}
#elif 0
		switch (key_state)
		{
			case key_state_0:				// 按键初始态
				if (key_input == KEY_ON) 		// 读按键I/O电平
					key_state = key_state_1;	// 键被按下，状态转换到键确认态
				break;
			case key_state_1:				// 按键确认态
				if (key_input == KEY_ON)
				{
					//key_return = 1;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
				else
					key_state = key_state_0;	// 按键已抬起，转换到按键初始态
				break;
			case key_state_2:
				if (key_input == KEY_ON){ 
					if(++timekey>100){ 
							timekey = 0;
							key_return = 2;
							key_state = key_state_3;
					 }
				}
				else{
					key_return = 1;
					timekey = 0;
					key_state = key_state_0;
				}
				break;
			case key_state_3:
				if (key_input == KEY_ON){ 
					if(++timekey>50){ 
							key_return = 2;
							timekey = 0;
					 }
				}
				else{
					timekey = 0;
					key_state = key_state_0;
				}
		}
#elif 0
	switch (key_state)
	{
		case key_state_0:				// 按键初始态
			if (key_inputN != ALLKEYOFF) 		// 读按键I/O电平
				key_state = key_state_1;	// 键被按下，状态转换到键确认态
			break;
		case key_state_1:				// 按键确认态
			if (key_inputN != ALLKEYOFF)
			{
				if(key_input1 == KEY_ON)
				{
					key_return = KEY1;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
				if(key_input2 == KEY_ON)
				{
					key_return = KEY2;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
				if(key_input3 == KEY_ON)
				{
					key_return = KEY3;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
				if(key_input4 == KEY_ON)
				{
					key_return = KEY4;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
			}
			else 
				key_state = key_state_0;	// 按键已抬起，转换到按键初始态
			break;
		case key_state_2:
			if (key_inputN == ALLKEYOFF){ //按键释放
			  key_state = key_state_0;	
			}
			break;
	}	
#elif 1
	switch (key_state)
	{
		case key_state_0:				// 按键初始态
			GPIO_ResetBits(KEYL_GPIO_PORT, KEYL1_GPIO_PIN|KEYL2_GPIO_PIN|KEYL3_GPIO_PIN);
			if (key_inputN != ALLKEYOFF) 		// 读按键I/O电平
				key_state = key_state_1;	// 键被按下，状态转换到键确认态
			break;
		case key_state_1:				// 按键确认态
			if (key_inputN != ALLKEYOFF)
			{
				if(key_input1 == KEY_ON)
				{
					key_return = KEY1;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
				if(key_input2 == KEY_ON)
				{
					key_return = KEY2;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
				if(key_input3 == KEY_ON)
				{
					key_return = KEY3;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
				if(key_input4 == KEY_ON)
				{
					key_return = KEY4;			// 按键仍按下，按键确认输出为"1" （1）
					key_state = key_state_2;	// 状态转换到键释放态
				}
			}
			else 
				key_state = key_state_0;	// 按键已抬起，转换到按键初始态
			break;
		case key_state_2:
			if (key_inputN == ALLKEYOFF){ //按键释放
			  key_state = key_state_0;	
			}
			break;
	}	
#endif
    return key_return;	
}


const uint16_t ROW_PINS[ROW_NUM] = {KEYL1_GPIO_PIN, KEYL2_GPIO_PIN, KEYL3_GPIO_PIN};
const uint16_t COL_PINS[COL_NUM] = {KEY1_GPIO_PIN, KEY2_GPIO_PIN, KEY3_GPIO_PIN, KEY4_GPIO_PIN};

uint8_t KeyBoard::ReadKeyN(void)
{
	uint8_t key_return = 0;
	switch (key_state)
	{
		case key_state_0:				// 按键初始态
			GPIO_ResetBits(KEYL_GPIO_PORT, KEYL1_GPIO_PIN|KEYL2_GPIO_PIN|KEYL3_GPIO_PIN);
			if (key_inputN != ALLKEYOFF) 		// 读按键I/O电平
				key_state = key_state_1;	// 键被按下，状态转换到键确认态
			break;
		case key_state_1:				// 按键确认态
			if (key_inputN != ALLKEYOFF)
			{
				GPIO_SetBits(KEYL_GPIO_PORT, KEYL1_GPIO_PIN|KEYL2_GPIO_PIN|KEYL3_GPIO_PIN);
				for(uint8_t i=0; i<3; i++) {
					GPIO_WriteBit(KEYL_GPIO_PORT, ROW_PINS[i], Bit_RESET);
					if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT, COL_PINS[0]) == Bit_RESET) 
							key_return = i*4 + 1;
					if(GPIO_ReadInputDataBit(KEY2_GPIO_PORT, COL_PINS[1]) == Bit_RESET) 
							key_return = i*4 + 2;
					if(GPIO_ReadInputDataBit(KEY3_GPIO_PORT, COL_PINS[2]) == Bit_RESET) 
							key_return = i*4 + 3;
					if(GPIO_ReadInputDataBit(KEY4_GPIO_PORT, COL_PINS[3]) == Bit_RESET) 
							key_return = i*4 + 4;
					// 其他列检测类似...
					GPIO_WriteBit(KEYL_GPIO_PORT, ROW_PINS[i], Bit_SET);
				}
				GPIO_ResetBits(KEYL_GPIO_PORT, KEYL1_GPIO_PIN|KEYL2_GPIO_PIN|KEYL3_GPIO_PIN);
				key_state = key_state_2;
			}
			else 
				key_state = key_state_0;	// 按键已抬起，转换到按键初始态
			break;
		case key_state_2:
			if (key_inputN == ALLKEYOFF){ //按键释放
			  key_state = key_state_0;	
			}
			break;
	}	
	return key_return;	
}

