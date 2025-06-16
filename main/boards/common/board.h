#pragma once

#include "led.h"
#include "display.h"
#include "lcd_display.h"
#include "oled_display.h"

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
    virtual LcdDisplay *GetLcdDisplay();
    virtual OledDisplay *GetOledDisplay();

  protected:
    Board();
};

#define DECLARE_BOARD(BOARD_CLASS_NAME)                                                                                \
    void *create_board() {                                                                                             \
        return new BOARD_CLASS_NAME();                                                                                 \
    }