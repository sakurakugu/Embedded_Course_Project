#include "board.h"

/*
 * 使用HSI时，设置系统时钟的步骤
 * 1、开启HSI ，并等待 HSI 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源，和PLL的倍频因子，设置各种频率主要就是在这里设置
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 */

/* 设置 系统时钟:SYSCLK, AHB总线时钟:HCLK, APB2总线时钟:PCLK2, APB1总线时钟:PCLK1
 * PCLK2 = HCLK = SYSCLK
 * PCLK1 = HCLK/2,最高只能是36M
 * 参数说明：pllmul是PLL的倍频因子，在调用的时候可以是：RCC_PLLMul_x , x:[2,3,...16]
 * 举例：HSI_SetSysClock(RCC_PLLMul_9);  则设置系统时钟为：4MHZ * 9 = 36MHZ
 *       HSI_SetSysClock(RCC_PLLMul_16); 则设置系统时钟为：4MHZ * 16 = 64MHZ
 *
 * HSI作为时钟来源，经过PLL倍频作为系统时钟，这是在HSE故障的时候才使用的方法
 * HSI会因为温度等原因会有漂移，不稳定，一般不会用HSI作为时钟来源，除非是迫不得已的情况
 * 如果HSI要作为PLL时钟的来源的话，必须二分频之后才可以，即HSI/2，而PLL倍频因子最大只能是16
 * 所以当使用HSI的时候，SYSCLK最大只能是4M*16=64M
 */

void HSI_SetSysClock(uint32_t pllmul) {
    __IO uint32_t HSIStartUpStatus = 0;

    // 把RCC外设初始化成复位状态，这句是必须的
    RCC_DeInit();

    // 使能HSI
    RCC_HSICmd(ENABLE);

    // 等待 HSI稳定
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET) {
    }
    // 读取 HSI 就绪状态
    HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;

    // 只有 HSI就绪之后则继续往下执行
    if (HSIStartUpStatus == RCC_CR_HSIRDY) {
        //----------------------------------------------------------------------//
        // 使能FLASH 预存取缓冲区
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

        // SYSCLK周期与闪存访问时间的比例设置，这里统一设置成2
        // 设置成2的时候，SYSCLK低于48M也可以工作，如果设置成0或者1的时候，
        // 如果配置的SYSCLK超出了范围的话，则会进入硬件错误，程序就死了
        // 0：0 < SYSCLK <= 24M
        // 1：24< SYSCLK <= 48M
        // 2：48< SYSCLK <= 72M
        FLASH_SetLatency(FLASH_Latency_2);
        //----------------------------------------------------------------------//

        // AHB预分频因子设置为1分频，HCLK = SYSCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        // APB2预分频因子设置为1分频，PCLK2 = HCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);

        // APB1预分频因子设置为1分频，PCLK1 = HCLK/2
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //-----------------设置各种频率主要就是在这里设置-------------------//
        // 设置PLL时钟来源为HSI，设置PLL倍频因子
        // PLLCLK = 4MHz * pllmul
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, pllmul);
        //------------------------------------------------------------------//

        // 开启PLL
        RCC_PLLCmd(ENABLE);

        // 等待 PLL稳定
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
        }

        // 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        // 读取时钟切换状态位，确保PLLCLK被选为系统时钟
        while (RCC_GetSYSCLKSource() != 0x08) {
        }
    } else { // 如果HSI开启失败，那么程序就会来到这里，用户可在这里添加出错的代码处理
             // 当HSE开启失败或者故障的时候，单片机会自动把HSI设置为系统时钟，
             // HSI是内部的高速时钟，8MHZ
        while (1) {
        }
    }
}


void dispdata(uint16_t count) {
    // count++;
    Disp_data[4] = count / 1000;
    Disp_data[5] = (count / 100) % 10;
    Disp_data[6] = count / 10 % 10;
    Disp_data[7] = count % 10;
}

/**
 * @brief  打印指令输入提示信息
 * @param  无
 * @retval 无
 */
void Show_Message(void) {
    printf("\r\n   这是一个通过串口通信指令控制RGB彩灯实验 \n");
    printf("使用  USART  参数为：%d 8-N-1 \n", DEBUG_USART_BAUDRATE);
    printf("开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
    printf("   指令   ------ 彩灯颜色 \n");
    printf("     1    ------    红 \n");
    printf("     2    ------    绿 \n");
    printf("     3    ------    蓝 \n");
    printf("     4    ------    黄 \n");
    printf("     5    ------    紫 \n");
    printf("     6    ------    青 \n");
    printf("     7    ------    白 \n");
    printf("     8    ------    灭 \n");
}

void test(uint8_t ch) {
    printf("接收到字符：%c\n", ch);

    /* 根据字符指令控制RGB彩灯颜色 */
    switch (ch) {
    case '1':
        LED_RED;
        break;
    case '2':
        LED_GREEN;
        break;
    case '3':
        LED_BLUE;
        break;
    case '4':
        LED_YELLOW;
        break;
    case '5':
        LED_PURPLE;
        break;
    case '6':
        LED_CYAN;
        break;
    case '7':
        LED_WHITE;
        break;
    case '8':
        LED_RGBOFF;
        break;
    default:
        /* 如果不是指定指令字符，打印提示信息 */
        Show_Message();
        break;
    }
}

Board::Board() {
    // 初始化代码
}

// Led *Board::GetLed() {
//     static NoLed led;
//     return &led;
// }

Key *Board::GetKey() {
    static NoKey key;
    return &key;
}

// LcdDisplay *Board::GetLcdDisplay() {
//     static NoLcdDisplay display;
//     return &display;
// }

// OledDisplay *Board::GetOledDisplay() {
//     static NoOledDisplay display;
//     return &display;
// }

// TouchScreen *Board::GetTouchScreen() {
//     return &TouchScreen::GetInstance();
// }

// Gui *Board::GetGui() {
//     return &Gui::GetInstance();
// }

// LedDisplay *Board::GetLedDisplay() {
//     static NoLedDisplay display;
//     return &display;
// }

