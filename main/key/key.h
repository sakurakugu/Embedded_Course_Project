#pragma once

#include <cstdint>

class Key {
public:
    virtual ~Key() = default;
    virtual uint8_t ReadKey() = 0; // 读取按键状态
    virtual uint8_t ReadKeyN() = 0; // 读取按键公共端口状态
};

class NoKey : public Key {
public:
    uint8_t ReadKey() override {
        return 0; // 返回默认值
    }

    uint8_t ReadKeyN() override {
        return 0;
    }
};