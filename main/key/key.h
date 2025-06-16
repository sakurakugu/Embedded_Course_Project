#pragma once

class Key {
public:
    virtual ~Key() = default;
    virtual void Init() = 0; // 初始化按键GPIO端口
    virtual uint8_t ReadKey() = 0; // 读取按键状态
    virtual void KeyComConfig() = 0; // 配置按键公共端口
};

class NoKey : public Key {
public:
    void Init() override {
    } // 不执行任何操作

    uint8_t ReadKey() override {
        return 0; // 返回默认值
    }

    void KeyComConfig() override {
    } // 不执行任何操作
};