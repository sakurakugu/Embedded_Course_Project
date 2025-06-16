#include "board.h"

Board::Board() {
    // 初始化代码
}

Led *Board::GetLed() {
    static NoLed led;
    return &led;
}

LcdDisplay *Board::GetLcdDisplay() {
    static NoLcdDisplay display;
    return &display;
}

OledDisplay *Board::GetOledDisplay() {
    static NoOledDisplay display;
    return &display;
}
