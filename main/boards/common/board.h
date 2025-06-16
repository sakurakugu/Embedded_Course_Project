#pragma once

#include "led.h"
#include "display.h"
#include "key.h"
#include "lcd_display.h"
#include "oled_display.h"
#include "touch.h"
#include "gui.h"
#include "led_display.h"

// 前向声明
class LcdDisplay;
class LedDisplay;
class OledDisplay;
class TouchScreen;
class Gui;
class Key;

void *create_board();
class Board {
  public:
    // 单例模式
    static Board &GetInstance() {
        static Board *instance = static_cast<Board *>(create_board());
        return *instance;
    }
    Board(const Board &) = delete;
    Board &operator=(const Board &) = delete;

    virtual ~Board() = default;
    virtual Led *GetLed();
    virtual Key *GetKey();
    virtual LcdDisplay *GetLcdDisplay();
    virtual OledDisplay *GetOledDisplay();
    virtual TouchScreen *GetTouchScreen();
    virtual Gui *GetGui();
    virtual LedDisplay *GetLedDisplay();

  protected:
    Board();
};

#define DECLARE_BOARD(BOARD_CLASS_NAME)                                                                                \
    void *create_board() {                                                                                             \
        return new BOARD_CLASS_NAME();                                                                                 \
    }