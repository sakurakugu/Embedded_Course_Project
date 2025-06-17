#include "application.h"

// 添加缺失的符号定义
extern "C" {
void _atexit_init(void) {
}
void _atexit_mutex(void) {
}
}

int main(void) {

    // 启动应用程序
    Application::GetInstance().Start();
}
