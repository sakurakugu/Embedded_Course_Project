#include "application.h"



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
}
/*********************************************END OF FILE**********************/
