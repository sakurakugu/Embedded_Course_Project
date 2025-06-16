#pragma once

class Led {
  public:
    virtual ~Led() = default;
    virtual void OnStateChanged() = 0; // 根据设备状态设置Led状态
};

class NoLed : public Led {
  public:
    void OnStateChanged() override {
    }
};