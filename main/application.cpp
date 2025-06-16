#include "application.h"

Application::Application() {
    // 私有构造函数
}

Application::~Application() {
    // 私有析构函数
}

void Application::Start() {
    auto &board = Board::GetInstance(); // 获取板子实例
    // 进入主事件循环
    MainEventLoop();
}

void Application::MainEventLoop() {
    uint8_t JQ8900Buff[] = {0xAA, 0X07, 0X02, 0X00, 0X00, 0X00};
    uint32_t time = 0, Key_time = 0, ESPReset = 0;
    uint8_t KeyNum = 0, MotorSpeed = 170;
    float temprature = 0, Rt, Temp;
    uint16_t adcValue;

    auto &board = Board::GetInstance();         // 获取板子实例
    auto lcd_display = board.GetLcdDisplay();   // 获取LCD显示屏实例
    auto oled_display = board.GetOledDisplay(); // 获取OLED显示屏实例
    auto led = board.GetLed(); // 直接使用Led基类指针
    auto touch_screen = board.GetTouchScreen(); // 获取触摸屏实例
    auto key = board.GetKey();                  // 获取按键实例
    auto led_display = board.GetLedDisplay(); // 获取LED显示屏实例

    printf("OLED测试例程\r\n");		//lm75A软件模拟i2c测试例程
	oled_display->ShowCHStr(0,0,(u8 *)"我们");
	oled_display->ShowNum(30,0,3251546,7,16);//不支持自动换行
	oled_display->ShowString(0,2,(u8 *)"hello");//不够自动换行
	oled_display->ShowChar(70,2,(u8)'A');//页地址模式，x坐标回0
	oled_display->ShowChinese(90,2,1);//显示第1个汉字 
	oled_display->DrawBMP(20,4,64,32,(u8 *)BMP1);
    while (true) {
        if (bsp_RunPer1ms == 1) {
            bsp_RunPer1ms = 0;
            led_display->FlashSegment();
            led_display->FlashDotMatrix();
            touch_screen->Test(); // 测试触摸屏
        }

        if (bsp_RunPer10ms == 1) {
            bsp_RunPer10ms = 0;
            KeyNum = key->ReadKey();

            if (KeyNum == 1) {
                led->ToggleLed3();
                JQ8900Buff[0] = 1;
                JQ8900Buff[1] = 2;
                JQ8x00_ZuHeBoFang(JQ8900Buff, 2);
                // printf("按键 %d 按下\r\n",KeyNum);
                if (MotorSpeed >= 250)
                    MotorSpeed = 170;
                MotorSpeed = MotorSpeed + 10;
                TIM_SetCompare2(TIM4, MotorSpeed);
                TIM_SetCompare3(TIM4, MotorSpeed);
            }
            if (KeyNum == 2) {
                led->ToggleLed2();
                JQ8x00_AppointPlay(JQ8X00_FLASH, const_cast<char *>("01"));
                DCMotor_Dir_Config(1);
            }
            if (KeyNum == 3) {
                led->ToggleLed2();
                // JQ8x00_Command_Data(AppointTrack,2);
                temprature = LM75_readTemp();
                // temprature = -10.6;
                printf("当前温度是 %.3f 度\r\n", temprature);
                DCMotor_Dir_Config(0);
            }
            if (KeyNum == 4) {
                led->ToggleLed2();
                // JQ8x00_Command_Data(SetVolume, 30);
                ee_Test(); // EEPROM测试
            }
        }
        if (bsp_RunPer1s == 1) {
            bsp_RunPer1s = 0;

            adcValue = getAdc(RV_ADC_CHANNEL);
            // printf("RV_ADC值是 %d\r\n", adcValue);
            adcValue = getAdc(NTC_ADC_CHANNEL);

            // printf("NTC_ADC值是 %d\r\n", adcValue);
            Rt = adcValue * 10000UL / (4095 - adcValue);
            Temp = 1 / (log(Rt / 10000) / 3950 + 1 / (25 + 273.15)) - 273.15;
            // printf("NTC_温度是 %.3f\r\n", Temp);

            E8266inittotlink();
            // printf("1S\r\n");
            if ((timensflag == 1) && (linkokflag == 1)) {
                // Txto8266Buf[1] = ;

                ESP_SendString(ESP_USARTx, (char *)Txto8266Buf);
                timensflag = 0;
            }
        }
    }
}