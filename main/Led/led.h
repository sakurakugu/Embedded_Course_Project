#pragma once

class Led {
  public:
    virtual ~Led() = default;
    virtual void OnStateChanged() = 0; // 根据设备状态设置Led状态
    virtual void ToggleLed1() = 0;     // 反转LED1状态
    virtual void ToggleLed2() = 0;     // 反转LED2状态
    virtual void ToggleLed3() = 0;     // 反转LED3状态
};

class NoLed : public Led {
  public:
    void OnStateChanged() override {
    }
    void ToggleLed1() override {
    }
    void ToggleLed2() override {
    }
    void ToggleLed3() override {
    }
};