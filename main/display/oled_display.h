#pragma once

#include "display.h"

class OledDisplay : public Display {
  public:
    OledDisplay();
    ~OledDisplay();
};

class NoOledDisplay : public OledDisplay {
  public:
    NoOledDisplay() : OledDisplay() {
    }
    ~NoOledDisplay() {
    }
};
