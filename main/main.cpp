#include "board.h"
#include "application.h"

// unsigned char JQ8900Buff[] = {0xAA, 0X07, 0X02, 0X00, 0X00, 0X00};

// 添加缺失的符号定义
extern "C" {
void _atexit_init(void) {}
void _atexit_mutex(void) {}
}

int main(void) {
    // 启动应用程序
    Application::GetInstance().Start();
    
    return 0;
}

