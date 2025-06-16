#include "application.h"
#include "board.h"
#include "bsp.h"
#include "bsp_SysTick.h"
#include "bsp_spi_bus.h"
#include "bsp_AdvanceTim.h"
#include "bsp_GeneralTim.h"
#include "bsp_adc.h"
#include "bsp_ESPxx.h"
#include "bsp_i2c_ee.h"
#include "stm32f10x.h"

// 添加缺失的符号定义
extern "C" {
    void _atexit_init(void) {}
    void _atexit_mutex(void) {}
}

// void SetRGBColor(uint32_t rgb) {
//     // 根据颜色值修改定时器的比较寄存器值
//     TIM_SetCompare1(TIM8, (uint8_t)rgb);
//     TIM_SetCompare2(TIM8, (uint8_t)(rgb >> 16));
//     TIM_SetCompare3(TIM8, (uint8_t)(rgb >> 8));
//     // COLOR_TIMx->COLOR_RED_CCRx = (uint8_t)(rgb>>16);      //R
//     // COLOR_TIMx->COLOR_GREEN_CCRx = (uint8_t)(rgb>>8);     //G
//     // COLOR_TIMx->COLOR_BLUE_CCRx = (uint8_t)rgb;           //B
// }

/**
 * @brief  主函数
 * @param  无
 * @retval 无
 */
int main(void) {

    // SetRGBColor(0x0000FF);

    // 启动应用程序
    Application::GetInstance().Start();

    // 永远不会到达这里
    while (1)
        ;
}
/*********************************************END OF FILE**********************/
