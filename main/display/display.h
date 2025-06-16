#pragma once

class Display {
  public:
    Display();
    virtual ~Display();
};

class NoDisplay : public Display {
  public:
    NoDisplay() : Display() {
    }
    ~NoDisplay() override {
    }
};