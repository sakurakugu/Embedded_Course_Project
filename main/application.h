#pragma once

#include "board.h"
#include "bsp_led.h"



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

    void Start(); // 启动应用程序

    // 菜单函数
    void Menu_Init(void);             // 初始化
    void Menu_Update(void);           // 更新
    void Menu_HandleKey(uint8_t key); // 处理按键
    void Menu_Display(void);          // 显示

    // 功能函数
    void Timer_Init(void);             // 初始化
    void Timer_Update(void);           // 更新
    void Timer_HandleKey(uint8_t key); // 处理按键

    void LEDControl_Init(void);             // 初始化
    void LEDControl_Update(void);           // 更新
    void LEDControl_HandleKey(uint8_t key); // 处理按键

    void FanControl_Init(void);             // 初始化
    void FanControl_Update(void);           // 更新
    void FanControl_HandleKey(uint8_t key); // 处理按键

    void About_Init(void);             // 初始化
    void About_Update(void);           // 更新
    void About_HandleKey(uint8_t key); // 处理按键

private:
    Application();  // 私有构造函数
    ~Application(); // 私有析构函数

    // 菜单项结构体
    typedef struct {
        const char *name;       // 菜单项名称
        void (*function)(void); // 菜单项对应的功能函数
    } MenuItem;

    // 菜单状态枚举
    typedef enum {
        MENU_MAIN,        // 主菜单
        MENU_TIMER,       // 计时器
        MENU_LED_CONTROL, // 彩灯控制
        MENU_FAN_CONTROL, // 风扇控制
        MENU_ABOUT        // 关于
    } MenuState;

    void MainEventLoop(); // 主事件循环

    // 全局变量
    MenuState currentState;
    uint8_t currentSelection;
    uint8_t menuItemCount;

    // 主菜单项
    static const MenuItem mainMenuItems[];

    // 计时器相关变量
    uint32_t timerValue;
    uint8_t timerRunning;

    // LED控制相关变量
    uint8_t ledState[3]; // 0: 红, 1: 绿, 2: 蓝的状态
    uint8_t currentLedColor;    // 0: 红, 1: 绿, 2: 蓝
    uint32_t currentRGB; // 当前RGB颜色值

    // 关于内容滚动相关变量
    uint8_t aboutScrollPos;
    uint16_t aboutScrollTimer;
    uint8_t aboutMaxLines;

    // 风扇控制相关变量
    uint8_t fanState;     // 风扇状态：0-关闭，1-开启
};
