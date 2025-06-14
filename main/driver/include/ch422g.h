#pragma once

#include "i2c_wrapper.h"
#include <stdint.h>


class CH422G {
public:
    enum Direct : uint8_t {
        IN = 0,
        OUT,
    };

    CH422G(uint8_t host);

    int set_direction(Direct dir);

    uint8_t read_input_reg();

    int write_output_reg(uint8_t value);

    int set_output_bit(uint8_t pin, uint8_t level);
    int get_input_bit(uint8_t pin);
private:
    Wrapper::I2C::Device _setting_reg;
    Wrapper::I2C::Device _input_reg;
    Wrapper::I2C::Device _output_reg;
    uint8_t _reg_val;
};
