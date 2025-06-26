#pragma once


#include "bsp_led.h"
#include "key_board.h"

#include "bsp_segdz.h"
#include "bsp_JQ8900.h"
#include "bsp_tlink.h"

#include "bsp_i2c_ee.h"
#include "bsp_i2c_lm75.h"

#include "bsp_oled.h"
#include "touch.h" 

#include "bsp_adc.h"
#include "bsp_GeneralTim.h" 
#include "bsp_AdvanceTim.h" 

#include "stm32f10x_it.h"
#include "misc.h"
#include "bsp_exti.h"



/* 提供给其他C文件调用的函数 */
	
void HSI_SetSysClock(uint32_t pllmul);

void dispdata(uint16_t count);
void Show_Message(void);

void test(uint8_t ch);


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
    // virtual Led *GetLed();
    virtual Key *GetKey();
    // virtual LcdDisplay *GetLcdDisplay();
    // virtual OledDisplay *GetOledDisplay();
    // virtual TouchScreen *GetTouchScreen();
    // virtual Gui *GetGui();
    // virtual LedDisplay *GetLedDisplay();

  protected:
    Board();
};

#define DECLARE_BOARD(BOARD_CLASS_NAME) \
    void *create_board() { \
        return new BOARD_CLASS_NAME(); \
    }