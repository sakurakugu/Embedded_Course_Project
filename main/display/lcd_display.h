#pragma once

#include "display.h"

class LcdDisplay : public Display {
  public:
    LcdDisplay();
    ~LcdDisplay();
};

class NoLcdDisplay : public LcdDisplay {
  public:
    NoLcdDisplay() : LcdDisplay() {
    }
    ~NoLcdDisplay() {
    }
};