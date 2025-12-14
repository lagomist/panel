#pragma once

#include "i2c_wrapper.h"
#include "gpio_wrapper.h"
#include <stdint.h>


class GT911 {
public:
    struct Coord {
        uint16_t x;
        uint16_t y;
        uint16_t strength;
    };

    GT911(uint8_t host, Wrapper::GPOBase &rst, Wrapper::GPOBase &irq) : _device(host), _rst(rst), _irq(irq) {}
    int init(uint32_t x_max, uint32_t y_max);
    void reset();
    int write_register(uint16_t reg, uint8_t data);
    int read_register(uint16_t reg, uint8_t buf[], uint32_t len);
    int read_data();
    int get_coordinates(uint16_t *x, uint16_t *y, uint16_t *strength, uint8_t max_point_num);
    int get_button_state();
private:
    constexpr static uint16_t GT911_TOUCH_MAX_POINTS    = 5;
    constexpr static uint16_t GT911_TOUCH_MAX_BUTTONS   = 1;
    
    Wrapper::I2C::Device _device;
    Wrapper::GPOBase &_rst;
    Wrapper::GPOBase &_irq;
    uint32_t _x_max;
    uint32_t _y_max;
    uint8_t _points;
    uint8_t _button;
    Coord _coords[GT911_TOUCH_MAX_POINTS];
};

