#include "application.h"

// 定义静态成员变量
const Application::MenuItem Application::mainMenuItems[] = {{"计时器", NULL}, {"彩灯控制", NULL}, {"风扇控制", NULL}, {"关于", NULL}};

// Application类实现
Application::Application() {
    // 构造函数实现 - 初始化成员变量
    currentState = MENU_MAIN;
    currentSelection = 0;
    menuItemCount = 4;
    
    // 计时器相关变量初始化
    timerValue = 0;
    timerRunning = 0;
    
    // LED控制相关变量初始化
    ledState[0] = 0;
    ledState[1] = 0;
    ledState[2] = 0;
    currentLedColor = 0;
    currentRGB = 0x000000;
    
    // 关于内容滚动相关变量初始化
    aboutScrollPos = 0;
    aboutScrollTimer = 0;
    aboutMaxLines = 6;
    
    // 风扇控制相关变量初始化
    fanState = 0;
}

Application::~Application() {
    // 析构函数实现
}

void Application::Start() {
    // 初始化BSP
    auto &board = Board::GetInstance();

    // 在LCD上显示学号和姓名
    LCD_Clear(WHITE);
    POINT_COLOR = BLACK;
    BACK_COLOR = WHITE;
    Show_Str(10, 10, BLUE, WHITE, (uint8_t *)"学号：2023030103024", 16, 0);
    Show_Str(10, 40, RED, WHITE, (uint8_t *)"姓名：潘彦玮", 16, 0);
    // delay_ms(3000); // 显示3秒

    // 初始化菜单系统
    Menu_Init();
    
    // 进入主事件循环
    MainEventLoop();
}

void Application::MainEventLoop() {
    uint8_t KeyNum = 0;
    auto &board = Board::GetInstance();
    auto key = board.GetKey();
    
    while (1) {
        if (bsp_RunPer1ms == 1) {
            bsp_RunPer1ms = 0;
            segFlash();
            dzFlash();
            TP_test();
        }

        if (bsp_RunPer10ms == 1) {
            bsp_RunPer10ms = 0;
            KeyNum = key->ReadKey();

            if (KeyNum != 0) {
                printf("按下按键%d\r\n", KeyNum);
                Menu_HandleKey(KeyNum);
            }
        }

        if (bsp_RunPer1s == 1) {
            bsp_RunPer1s = 0;
            Menu_Update();
        }
    }
}

// 菜单初始化
void Application::Menu_Init(void) {
    currentState = MENU_MAIN;
    currentSelection = 0;
    oled_Clear();
}

// 菜单更新
void Application::Menu_Update(void) {
    switch (currentState) {
    case MENU_MAIN:
        Menu_Display();
        break;
    case MENU_TIMER:
        Timer_Update();
        break;
    case MENU_LED_CONTROL:
        LEDControl_Update();
        break;
    case MENU_FAN_CONTROL:
        FanControl_Update();
        break;
    case MENU_ABOUT:
        About_Update();
        break;
    }
}

// 菜单显示
void Application::Menu_Display(void) {
    oled_Clear();
    oled_ShowString(0, 0, (uint8_t *)"主菜单");

    // 计算显示起始索引，确保当前选择的项目在可视区域内
    uint8_t startIdx = 0;
    if (currentSelection > 1) {
        startIdx = currentSelection - 1;
    }

    // 限制结束索引，最多显示3行选项
    uint8_t endIdx = startIdx + 3;
    if (endIdx > menuItemCount) {
        endIdx = menuItemCount;
    }

    // 显示菜单项
    for (uint8_t i = startIdx; i < endIdx; i++) {
        uint8_t displayRow = 2 + (i - startIdx) * 2; // 从第2行开始，每项占2行

        if (i == currentSelection) {
            oled_ShowAsciiString(0, displayRow, (uint8_t *)">");
        }
        oled_ShowString(8, displayRow, (uint8_t *)mainMenuItems[i].name);
    }
}

// 按键处理
void Application::Menu_HandleKey(uint8_t key) {
    switch (currentState) {
    case MENU_MAIN:
        if (key == 1) { // 上键
            if (currentSelection > 0) {
                currentSelection--;
            }
        } else if (key == 2) { // 下键
            if (currentSelection < menuItemCount - 1) {
                currentSelection++;
            }
        } else if (key == 3) { // 确认键
            switch (currentSelection) {
            case 0:
                currentState = MENU_TIMER;
                Timer_Init();
                break;
            case 1:
                currentState = MENU_LED_CONTROL;
                LEDControl_Init();
                break;
            case 2:
                currentState = MENU_FAN_CONTROL;
                FanControl_Init();
                break;
            case 3:
                currentState = MENU_ABOUT;
                About_Init();
                break;
            }
        }
        break;

    case MENU_TIMER:
        Timer_HandleKey(key);
        break;

    case MENU_LED_CONTROL:
        LEDControl_HandleKey(key);
        break;

    case MENU_FAN_CONTROL:
        FanControl_HandleKey(key);
        break;

    case MENU_ABOUT:
        About_HandleKey(key);
        break;
    }
}

// 计时器功能实现
void Application::Timer_Init(void) {
    timerValue = 0;
    timerRunning = 0;
    oled_Clear();
    oled_ShowString(0, 0, (uint8_t *)"计时器");
}

void Application::Timer_Update(void) {
    if (timerRunning) {
        timerValue++;
    }

    uint32_t hours = timerValue / 3600;
    uint32_t minutes = (timerValue % 3600) / 60;
    uint32_t seconds = timerValue % 60;

    oled_ShowNum(0, 2, hours, 2, 16);
    oled_ShowAsciiString(16, 2, (uint8_t *)":");
    oled_ShowNum(24, 2, minutes, 2, 16);
    oled_ShowAsciiString(40, 2, (uint8_t *)":");
    oled_ShowNum(48, 2, seconds, 2, 16);
}

void Application::Timer_HandleKey(uint8_t key) {
    if (key == 4) { // 返回键
        currentState = MENU_MAIN;
        return;
    }

    if (key == 3) { // 确认键
        timerRunning = !timerRunning;
    }
}

// LED控制功能实现
void Application::LEDControl_Init(void) {
    currentLedColor = 0;
    oled_Clear();
    oled_ShowString(0, 0, (uint8_t *)"彩灯控制");
}

void Application::LEDControl_Update(void) {

    oled_ShowString(0, 4, (uint8_t *)"当前控制: ");
    switch (currentLedColor) {
    case 0:
        oled_ShowString(64, 4, (uint8_t *)"红灯");
        break;
    case 1:
        oled_ShowString(64, 4, (uint8_t *)"绿灯");
        break;
    case 2:
        oled_ShowString(64, 4, (uint8_t *)"蓝灯");
        break;
    }

    oled_ShowString(0, 2, (uint8_t *)"彩灯状态: ");
    oled_ShowString(64, 2, ledState[currentLedColor] ? (uint8_t *)"开" : (uint8_t *)"关");
}

void Application::LEDControl_HandleKey(uint8_t key) {
    if (key == 4) { // 返回键
        currentState = MENU_MAIN;
        return;
    }

    if (key == 3) { // 确认键
        ledState[currentLedColor] = !ledState[currentLedColor];
        if (ledState[currentLedColor]) {
            // 根据当前选择的颜色设置LED
            switch (currentLedColor) {
            case 0:                                              // 红色
                currentRGB = (currentRGB & 0x00FFFF) | 0xFF0000; // 只修改红色分量
                break;
            case 1:                                              // 绿色
                currentRGB = (currentRGB & 0xFF00FF) | 0x00FF00; // 只修改绿色分量
                break;
            case 2:                                              // 蓝色
                currentRGB = (currentRGB & 0xFFFF00) | 0x0000FF; // 只修改蓝色分量
                break;
            }
        } else {
            switch (currentLedColor) {
            case 0:                                              // 红色
                currentRGB = (currentRGB & 0x00FFFF) | 0x000000; // 关闭红色分量
                break;
            case 1:                                              // 绿色
                currentRGB = (currentRGB & 0xFF00FF) | 0x000000; // 关闭绿色分量
                break;
            case 2:                                              // 蓝色
                currentRGB = (currentRGB & 0xFFFF00) | 0x000000; // 关闭蓝色分量
                break;
            }
        }
        SetRGBColor(currentRGB);
    } else if (key == 1) {                           // 上键
        currentLedColor = (currentLedColor + 2) % 3; // 循环选择颜色（向上）
    } else if (key == 2) {                           // 下键
        currentLedColor = (currentLedColor + 1) % 3; // 循环选择颜色（向下）
    }
}

// 关于功能实现
void Application::About_Init(void) {
    oled_Clear();
    oled_ShowString(0, 0, (uint8_t *)"关于");
    aboutScrollPos = 0;
    aboutScrollTimer = 0;
}

void Application::About_Update(void) {
    aboutScrollTimer++;
    if (aboutScrollTimer >= 100) { // 调整滚动速度
        aboutScrollTimer = 0;
        aboutScrollPos++;
        if (aboutScrollPos > aboutMaxLines - 3) { // 显示3行，所以最大滚动位置是总行数-3
            aboutScrollPos = 0;
        }
    }
    // 清除屏幕，重新显示标题和内容
    oled_Clear();
    oled_ShowString(0, 0, (uint8_t *)"关于");

    // 根据滚动位置显示内容
    const char *aboutContent[] = {"STM32菜单演示程序", "版本: V1.0", "作者: 潘彦玮",
                                  "学号: 20230301",    "     03024", "时间: 2025-06-18"};

    // 显示当前可见的3行内容
    for (uint8_t i = 0; i < 3 && (i + aboutScrollPos) < aboutMaxLines; i++) {
        oled_ShowString(0, 2 + i * 2, (uint8_t *)aboutContent[i + aboutScrollPos]);
    }
}

// void Application::About_Update(void) {
//     aboutScrollTimer++;
//     if (aboutScrollTimer >= 100) { // 调整滚动速度
//         aboutScrollTimer = 0;
//         aboutScrollPos = (aboutScrollPos + 1) % aboutMaxLines;
//     }

//     // 显示关于信息
//     const char *aboutLines[] = {
//         "STM32菜单系统",
//         "版本: 1.0",
//         "作者: 学生",
//         "日期: 2023-06-01",
//         "功能: 演示菜单",
//         "按返回键退出"
//     };

//     // 显示当前页和总页数
//     oled_ShowString(0, 6, (uint8_t *)"页码:");
//     oled_ShowNum(48, 6, aboutScrollPos + 1, 1, 16);
//     oled_ShowAsciiString(56, 6, (uint8_t *)"/");
//     oled_ShowNum(64, 6, aboutMaxLines, 1, 16);

//     // 显示当前页内容
//     oled_ShowString(0, 2, (uint8_t *)aboutLines[aboutScrollPos]);
// }


void Application::About_HandleKey(uint8_t key) {
    if (key == 4) { // 返回键
        currentState = MENU_MAIN;
        return;
    }

    if (key == 1) { // 上键
        if (aboutScrollPos > 0) {
            aboutScrollPos--;
        } else {
            aboutScrollPos = aboutMaxLines - 3; // 循环到底部
        }
        aboutScrollTimer = 0; // 重置自动滚动计时器
    } else if (key == 2) {    // 下键
        if (aboutScrollPos < aboutMaxLines - 3) {
            aboutScrollPos++;
        } else {
            aboutScrollPos = 0; // 循环到顶部
        }
        aboutScrollTimer = 0; // 重置自动滚动计时器
    } else if (key == 3) {    // 主页键
        currentState = MENU_MAIN;
        return;
    }
}

// void Application::About_HandleKey(uint8_t key) {
//     if (key == 4) { // 返回键
//         currentState = MENU_MAIN;
//         return;
//     }

//     if (key == 1) { // 上键
//         aboutScrollPos = (aboutScrollPos + aboutMaxLines - 1) % aboutMaxLines;
//     } else if (key == 2) { // 下键
//         aboutScrollPos = (aboutScrollPos + 1) % aboutMaxLines;
//     }
// }

// 风扇控制功能实现
void Application::FanControl_Init(void) {
    fanState = 0;
    oled_Clear();
    oled_ShowString(0, 0, (uint8_t *)"风扇控制");
}

void Application::FanControl_Update(void) {
    oled_ShowString(0, 2, (uint8_t *)"风扇状态: ");
    oled_ShowString(64, 2, fanState ? (uint8_t *)"开" : (uint8_t *)"关");
}

void Application::FanControl_HandleKey(uint8_t key) {
    if (key == 4) { // 返回键
        currentState = MENU_MAIN;
        return;
    }

    if (key == 3) { // 确认键
        DCMotor_Dir_Config(fanState);
        fanState = !fanState;
    }
}
