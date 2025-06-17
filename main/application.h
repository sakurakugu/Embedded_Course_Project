#pragma once

#include <cstdint>
#include "board.h"
#include "stm32f10x_it.h"
#include "led_display.h"
#include "bsp_usart.h"
#include "bsp_JQ8900.h"
#include "lm75.h"
#include "eeprom.h"
#include "adc.h"
#include "general_tim.h"
#include "gui.h"
#include "touch.h"
#include "oled_display.h"
#include "bsp_ESPxx.h"
#include "bsp_tlink.h"

class Application {
  public:
    // 单例模式
    static Application &GetInstance() {
        static Application instance;
        return instance;
    }
    // 删除拷贝构造函数和赋值运算符
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void Start(); // 启动
  private:
    Application();  // 私有构造函数
    ~Application(); // 私有析构函数

    void MainEventLoop(); // 主事件循环
};