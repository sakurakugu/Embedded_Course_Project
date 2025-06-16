#include "board.h"
#include "lcd_display.h"
#include "oled_display.h"
#include "led.h"
#include "touch.h"
#include "gui.h"

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

TouchScreen *Board::GetTouchScreen() {
    return &TouchScreen::GetInstance();
}

Gui *Board::GetGui() {
    return &Gui::GetInstance();
}
